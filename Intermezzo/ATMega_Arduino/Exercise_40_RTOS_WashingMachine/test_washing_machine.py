"""
Washing Machine — Hardware-in-the-Loop Test Suite
==================================================
Requires: pytest, pyserial
Install:  pip install pytest pyserial

Usage:
    pytest test_washing_machine.py -v
    pytest test_washing_machine.py -v -k TC01

Hardware preconditions:
    - Arduino Uno flashed with debug build (make debug)
    - Door contact (D4) closed at test start
    - Potentiometer (A0) below threshold at test start
    - Serial port configured via PORT constant below

Notes:
    - Arduino resets when serial port opens — STARTUP_DELAY handles this
    - All physical sensors simulated via serial commands
    - Tests are sequential — each leaves machine in IDLE
"""

import pytest
import serial
import time

# ----- Configuration -----------------------------------------
PORT            = '/dev/cu.usbmodem101'
BAUD            = 9600
STARTUP_DELAY   = 5.0       # seconds after serial connect
TIMEOUT         = 10.0      # serial readline timeout
PHASE_DURATION  = 4.0       # slightly longer than 3s phase timers


# ----- Helpers -----------------------------------------------
class WashingMachine:
    def __init__(self, port, baud):
        self.ser = serial.Serial(port, baud, timeout=TIMEOUT)
        time.sleep(STARTUP_DELAY)
        self.ser.flushInput()

    def close(self):
        self.ser.close()

    def send(self, cmd: str):
        self.ser.write(f'{cmd}\r\n'.encode())

    def read_until(self, expected: str, timeout: float = TIMEOUT) -> bool:
        """Read lines until expected string found or timeout."""
        deadline = time.time() + timeout
        while time.time() < deadline:
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                print(f'  << {line}')
            if expected in line:
                return True
        return False

    def flush(self):
        self.ser.flushInput()

    def reset_to_idle(self):
        """Ensure machine is in IDLE state."""
        self.send('cancel')
        time.sleep(0.5)
        self.flush()


# ----- Fixtures ----------------------------------------------
@pytest.fixture(scope='module')
def wm():
    machine = WashingMachine(PORT, BAUD)
    yield machine
    machine.reset_to_idle()
    machine.close()

@pytest.fixture(autouse=True)
def reset_between_tests(wm):
    """Reset machine to IDLE before each test."""
    wm.reset_to_idle()
    yield
    time.sleep(0.2)


# ----- TC01 — Happy path: full wash cycle --------------------
def test_tc01_happy_path(wm):
    """Full wash cycle from IDLE to DONE."""
    wm.send('close')
    assert wm.read_until('EV_DOOR_CLOSED'), 'Door closed event not received'

    wm.send('start')
    assert wm.read_until('Filling'), 'Did not enter FILLING state'

    wm.send('water')
    assert wm.read_until('Washing'), 'Did not enter WASHING state'

    assert wm.read_until('Draining', timeout=PHASE_DURATION + 1), \
        'Did not enter DRAINING_AFTER_WASH'

    assert wm.read_until('Rinsing', timeout=PHASE_DURATION + 1), \
        'Did not enter RINSING'

    assert wm.read_until('Draining', timeout=PHASE_DURATION + 1), \
        'Did not enter DRAINING_AFTER_RINSE'

    assert wm.read_until('Spinning', timeout=PHASE_DURATION + 1), \
        'Did not enter SPINNING'

    assert wm.read_until('Done', timeout=PHASE_DURATION + 1), \
        'Did not enter DONE'


# ----- TC02 — Cancel mid-cycle -------------------------------
def test_tc02_cancel_mid_cycle(wm):
    """Cancel during WASHING returns to IDLE."""
    wm.send('close')
    wm.send('start')
    assert wm.read_until('Filling'), 'Did not enter FILLING'

    wm.send('water')
    assert wm.read_until('Washing'), 'Did not enter WASHING'

    wm.send('cancel')
    assert wm.read_until('IDLE'), 'Did not return to IDLE after cancel'

    wm.send('status')
    assert wm.read_until('IDLE'), 'Status should show IDLE'
    assert wm.read_until('NO_ERROR'), 'Error code should be NO_ERROR'


# ----- TC03 — Door open during wash (safety) -----------------
def test_tc03_door_open_during_wash(wm):
    """Door opening during WASHING triggers ERROR with safe actuator stop."""
    wm.send('close')
    wm.send('start')
    assert wm.read_until('Filling'), 'Did not enter FILLING'

    wm.send('water')
    assert wm.read_until('Washing'), 'Did not enter WASHING'

    wm.send('door')
    assert wm.read_until('Error'), 'Did not enter ERROR after door open'

    wm.send('status')
    assert wm.read_until('ERROR'), 'Status should show ERROR'
    assert wm.read_until('DOOR_OPEN'), 'Error code should be DOOR_OPEN'

    wm.send('cancel')
    assert wm.read_until('IDLE'), 'Did not recover to IDLE'


# ----- TC04 — Water timeout during filling -------------------
def test_tc04_water_timeout(wm):
    """Water level not reached within timeout triggers ERROR."""
    wm.send('close')
    wm.send('start')
    assert wm.read_until('Filling'), 'Did not enter FILLING'

    # Do NOT send 'water' — wait for timeout (15s)
    assert wm.read_until('Error', timeout=20.0), \
        'Did not enter ERROR after water timeout'

    wm.send('status')
    assert wm.read_until('WATER_TIMEOUT'), 'Error code should be WATER_TIMEOUT'

    wm.send('cancel')
    assert wm.read_until('IDLE'), 'Did not recover to IDLE'


# ----- TC05 — Start with door open ---------------------------
def test_tc05_start_with_door_open(wm):
    """Starting with door open triggers ERROR immediately."""
    wm.send('door')
    assert wm.read_until('EV_DOOR_OPEN'), 'Door open event not received'

    wm.send('start')
    assert wm.read_until('Error'), 'Did not enter ERROR with door open'

    wm.send('status')
    assert wm.read_until('START_WITH_DOOR_OPEN'), \
        'Error code should be START_WITH_DOOR_OPEN'

    wm.send('cancel')
    assert wm.read_until('IDLE'), 'Did not recover to IDLE'


# ----- TC06 — Motor fault ------------------------------------
def test_tc06_motor_fault(wm):
    """Motor error during WASHING triggers ERROR with safe stop."""
    wm.send('close')
    wm.send('start')
    assert wm.read_until('Filling'), 'Did not enter FILLING'

    wm.send('water')
    assert wm.read_until('Washing'), 'Did not enter WASHING'

    wm.send('fault')
    assert wm.read_until('Error'), 'Did not enter ERROR after motor fault'

    wm.send('status')
    assert wm.read_until('MOTOR_ERROR'), 'Error code should be MOTOR_ERROR'

    wm.send('cancel')
    assert wm.read_until('IDLE'), 'Did not recover to IDLE'


# ----- TC07 — Event queue overflow ---------------------------
def test_tc07_queue_overflow(wm):
    """Rapid commands trigger queue overflow detection."""
    # Flood the queue
    for _ in range(10):
        wm.ser.write(b'start\r\n')

    assert wm.read_until('QUEUE OVERFLOW', timeout=3.0), \
        'Queue overflow not detected'

    # System should recover
    wm.send('cancel')
    assert wm.read_until('IDLE', timeout=3.0), \
        'System did not recover after queue overflow'


# ----- TC08 — CPU load measurement --------------------------
def test_tc08_cpu_load(wm):
    """CPU idle count remains high during operation."""
    # Read idle count at rest
    idle_line = ''
    deadline = time.time() + 3.0
    while time.time() < deadline:
        line = wm.ser.readline().decode('utf-8', errors='ignore').strip()
        if 'CPU IDLE' in line:
            next_line = wm.ser.readline().decode('utf-8', errors='ignore').strip()
            idle_line = next_line
            break

    assert idle_line, 'No CPU IDLE output received'
    idle_count = int(idle_line)
    print(f'  Idle count: {idle_count}')
    assert idle_count > 30000, f'CPU load too high: idle count {idle_count}'

    # Now run a wash cycle and check load stays high
    wm.send('close')
    wm.send('start')
    wm.read_until('Filling')
    wm.send('water')
    wm.read_until('Washing')

    idle_line = ''
    deadline = time.time() + 3.0
    while time.time() < deadline:
        line = wm.ser.readline().decode('utf-8', errors='ignore').strip()
        if 'CPU IDLE' in line:
            next_line = wm.ser.readline().decode('utf-8', errors='ignore').strip()
            idle_line = next_line
            break

    assert idle_line, 'No CPU IDLE output during wash'
    idle_during_wash = int(idle_line)
    print(f'  Idle during wash: {idle_during_wash}')
    assert idle_during_wash > 30000, \
        f'CPU load too high during wash: {idle_during_wash}'