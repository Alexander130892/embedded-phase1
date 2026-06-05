#!/usr/bin/env python3
"""
Serial logger for STM32 stability testing.
Logs all UART output to a timestamped file and prints to terminal.

Usage:
    python3 serial_logger.py
    python3 serial_logger.py --port /dev/tty.usbmodem103 --baud 115200
    python3 serial_logger.py --port /dev/tty.usbmodem103 --baud 115200 --output my_test.log
"""

import serial
import datetime
import argparse
import sys

def main():
    parser = argparse.ArgumentParser(description="STM32 serial logger")
    parser.add_argument("--port",   default="/dev/tty.usbmodem103", help="Serial port")
    parser.add_argument("--baud",   default=115200, type=int,        help="Baud rate")
    parser.add_argument("--output", default="stability_test.log",    help="Output log file")
    args = parser.parse_args()

    print(f"Opening {args.port} at {args.baud} baud")
    print(f"Logging to {args.output}")
    print("Press Ctrl+C to stop\n")

    try:
        with serial.Serial(args.port, args.baud, timeout=1) as s, \
             open(args.output, "w") as f:

            while True:
                line = s.readline().decode("utf-8", errors="replace")
                if line:
                    ts = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]
                    entry = f"{ts}  {line}"
                    f.write(entry)
                    f.flush()
                    print(entry, end="")

    except serial.SerialException as e:
        print(f"\nSerial error: {e}", file=sys.stderr)
        sys.exit(1)
    except KeyboardInterrupt:
        print("\n\nStopped. Analysing log...")
        analyse(args.output)

def analyse(logfile):
    """Quick post-run analysis."""
    try:
        with open(logfile) as f:
            lines = f.readlines()
    except FileNotFoundError:
        print("Log file not found.")
        return

    total     = len(lines)
    errors    = [l for l in lines if "ERROR" in l]
    garbled   = [l for l in lines if not l[:8].replace(":", "").replace(".", "").isdigit()]

    print(f"\n--- Results ---")
    print(f"Total lines   : {total}")
    print(f"ERROR messages: {len(errors)}")
    if errors:
        print("  First error :", errors[0].strip())
    print(f"Garbled lines : {len(garbled)}")
    if garbled:
        print("  First garbled:", garbled[0].strip())
    print("---------------")
    if not errors and not garbled:
        print("PASS — no errors or corruption detected.")
    else:
        print("FAIL — review log file.")

if __name__ == "__main__":
    main()