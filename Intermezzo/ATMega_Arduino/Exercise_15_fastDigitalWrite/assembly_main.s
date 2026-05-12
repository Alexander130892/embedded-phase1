avr-objdump -d -S main.elf | less

main.elf:     file format elf32-avr


Disassembly of section .text:

00000000 <__vectors>:
   0:	0c 94 34 00 	jmp	0x68	; 0x68 <__ctors_end>
   4:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
   8:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
   c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  10:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  14:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  18:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  1c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  20:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  24:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  28:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  2c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  30:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  34:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  38:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  3c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  40:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  44:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  48:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  4c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  50:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  54:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  58:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  5c:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  60:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>
  64:	0c 94 3e 00 	jmp	0x7c	; 0x7c <__bad_interrupt>

00000068 <__ctors_end>:
  68:	11 24       	eor	r1, r1
  6a:	1f be       	out	0x3f, r1	; 63
  6c:	cf ef       	ldi	r28, 0xFF	; 255
  6e:	d8 e0       	ldi	r29, 0x08	; 8
  70:	de bf       	out	0x3e, r29	; 62
  72:	cd bf       	out	0x3d, r28	; 61
  74:	0e 94 53 00 	call	0xa6	; 0xa6 <main>
  78:	0c 94 07 01 	jmp	0x20e	; 0x20e <_exit>

0000007c <__bad_interrupt>:
  7c:	0c 94 00 00 	jmp	0	; 0x0 <__vectors>

00000080 <fastWrite7>:
  80:	88 23       	and	r24, r24
  82:	11 f0       	breq	.+4      	; 0x88 <.L2>
  84:	5f 9a       	sbi	0x0b, 7	; 11
  86:	08 95       	ret

00000088 <.L2>:
  88:	5f 98       	cbi	0x0b, 7	; 11
  8a:	08 95       	ret

0000008c <fastToggle>:
  8c:	83 70       	andi	r24, 0x03	; 3
  8e:	9b b1       	in	r25, 0x0b	; 11
  90:	8c 5f       	subi	r24, 0xFC	; 252
  92:	21 e0       	ldi	r18, 0x01	; 1
  94:	30 e0       	ldi	r19, 0x00	; 0
  96:	01 c0       	rjmp	.+2      	; 0x9a <.L2^B2>

00000098 <.L1^B2>:
  98:	22 0f       	add	r18, r18

0000009a <.L2^B2>:
  9a:	8a 95       	dec	r24
  9c:	ea f7       	brpl	.-6      	; 0x98 <.L1^B2>
  9e:	89 2f       	mov	r24, r25
  a0:	82 27       	eor	r24, r18
  a2:	8b b9       	out	0x0b, r24	; 11
  a4:	08 95       	ret

000000a6 <main>:
  a6:	cd b7       	in	r28, 0x3d	; 61
  a8:	de b7       	in	r29, 0x3e	; 62
  aa:	68 97       	sbiw	r28, 0x18	; 24
  ac:	0f b6       	in	r0, 0x3f	; 63
  ae:	f8 94       	cli
  b0:	de bf       	out	0x3e, r29	; 62
  b2:	0f be       	out	0x3f, r0	; 63
  b4:	cd bf       	out	0x3d, r28	; 61
  b6:	80 ef       	ldi	r24, 0xF0	; 240
  b8:	8a b9       	out	0x0a, r24	; 10
  ba:	8f e0       	ldi	r24, 0x0F	; 15
  bc:	8b b9       	out	0x0b, r24	; 11

000000be <.L22>:
  be:	5f 9a       	sbi	0x0b, 7	; 11
  c0:	1d 8a       	std	Y+21, r1	; 0x15
  c2:	1e 8a       	std	Y+22, r1	; 0x16
  c4:	1f 8a       	std	Y+23, r1	; 0x17
  c6:	18 8e       	std	Y+24, r1	; 0x18

000000c8 <.L10>:
  c8:	8d 89       	ldd	r24, Y+21	; 0x15
  ca:	9e 89       	ldd	r25, Y+22	; 0x16
  cc:	af 89       	ldd	r26, Y+23	; 0x17
  ce:	b8 8d       	ldd	r27, Y+24	; 0x18
  d0:	80 3a       	cpi	r24, 0xA0	; 160
  d2:	96 48       	sbci	r25, 0x86	; 134
  d4:	a1 40       	sbci	r26, 0x01	; 1
  d6:	b1 05       	cpc	r27, r1
  d8:	08 f4       	brcc	.+2      	; 0xdc <L0^A+0x2>

000000da <L0^A>:
  da:	5d c0       	rjmp	.+186    	; 0x196 <.L11>
  dc:	80 e0       	ldi	r24, 0x00	; 0
  de:	0e 94 40 00 	call	0x80	; 0x80 <fastWrite7>
  e2:	19 8a       	std	Y+17, r1	; 0x11
  e4:	1a 8a       	std	Y+18, r1	; 0x12
  e6:	1b 8a       	std	Y+19, r1	; 0x13
  e8:	1c 8a       	std	Y+20, r1	; 0x14

000000ea <.L12>:
  ea:	89 89       	ldd	r24, Y+17	; 0x11
  ec:	9a 89       	ldd	r25, Y+18	; 0x12
  ee:	ab 89       	ldd	r26, Y+19	; 0x13
  f0:	bc 89       	ldd	r27, Y+20	; 0x14
  f2:	80 3a       	cpi	r24, 0xA0	; 160
  f4:	96 48       	sbci	r25, 0x86	; 134
  f6:	a1 40       	sbci	r26, 0x01	; 1
  f8:	b1 05       	cpc	r27, r1
  fa:	08 f4       	brcc	.+2      	; 0xfe <.Lname42+0x1>

000000fc <L0^A>:
  fc:	58 c0       	rjmp	.+176    	; 0x1ae <.L13>
  fe:	5e 9a       	sbi	0x0b, 6	; 11
 100:	1d 86       	std	Y+13, r1	; 0x0d
 102:	1e 86       	std	Y+14, r1	; 0x0e
 104:	1f 86       	std	Y+15, r1	; 0x0f
 106:	18 8a       	std	Y+16, r1	; 0x10

00000108 <.L14>:
 108:	8d 85       	ldd	r24, Y+13	; 0x0d
 10a:	9e 85       	ldd	r25, Y+14	; 0x0e
 10c:	af 85       	ldd	r26, Y+15	; 0x0f
 10e:	b8 89       	ldd	r27, Y+16	; 0x10
 110:	80 3a       	cpi	r24, 0xA0	; 160
 112:	96 48       	sbci	r25, 0x86	; 134
 114:	a1 40       	sbci	r26, 0x01	; 1
 116:	b1 05       	cpc	r27, r1
 118:	08 f4       	brcc	.+2      	; 0x11c <L0^A+0x2>

0000011a <L0^A>:
 11a:	55 c0       	rjmp	.+170    	; 0x1c6 <.L15>
 11c:	5e 98       	cbi	0x0b, 6	; 11
 11e:	19 86       	std	Y+9, r1	; 0x09
 120:	1a 86       	std	Y+10, r1	; 0x0a
 122:	1b 86       	std	Y+11, r1	; 0x0b
 124:	1c 86       	std	Y+12, r1	; 0x0c

00000126 <.L16>:
 126:	89 85       	ldd	r24, Y+9	; 0x09
 128:	9a 85       	ldd	r25, Y+10	; 0x0a
 12a:	ab 85       	ldd	r26, Y+11	; 0x0b
 12c:	bc 85       	ldd	r27, Y+12	; 0x0c
 12e:	80 3a       	cpi	r24, 0xA0	; 160
 130:	96 48       	sbci	r25, 0x86	; 134
 132:	a1 40       	sbci	r26, 0x01	; 1
 134:	b1 05       	cpc	r27, r1
 136:	08 f4       	brcc	.+2      	; 0x13a <L0^A+0x2>

00000138 <L0^A>:
 138:	52 c0       	rjmp	.+164    	; 0x1de <.L17>
 13a:	85 e0       	ldi	r24, 0x05	; 5
 13c:	0e 94 46 00 	call	0x8c	; 0x8c <fastToggle>
 140:	1d 82       	std	Y+5, r1	; 0x05
 142:	1e 82       	std	Y+6, r1	; 0x06
 144:	1f 82       	std	Y+7, r1	; 0x07
 146:	18 86       	std	Y+8, r1	; 0x08

00000148 <.L18>:
 148:	8d 81       	ldd	r24, Y+5	; 0x05
 14a:	9e 81       	ldd	r25, Y+6	; 0x06
 14c:	af 81       	ldd	r26, Y+7	; 0x07
 14e:	b8 85       	ldd	r27, Y+8	; 0x08
 150:	80 3a       	cpi	r24, 0xA0	; 160
 152:	96 48       	sbci	r25, 0x86	; 134
 154:	a1 40       	sbci	r26, 0x01	; 1
 156:	b1 05       	cpc	r27, r1
 158:	08 f4       	brcc	.+2      	; 0x15c <L0^A+0x2>

0000015a <L0^A>:
 15a:	4d c0       	rjmp	.+154    	; 0x1f6 <.L19>
 15c:	85 e0       	ldi	r24, 0x05	; 5
 15e:	0e 94 46 00 	call	0x8c	; 0x8c <fastToggle>
 162:	19 82       	std	Y+1, r1	; 0x01
 164:	1a 82       	std	Y+2, r1	; 0x02
 166:	1b 82       	std	Y+3, r1	; 0x03
 168:	1c 82       	std	Y+4, r1	; 0x04

0000016a <.L20>:
 16a:	89 81       	ldd	r24, Y+1	; 0x01
 16c:	9a 81       	ldd	r25, Y+2	; 0x02
 16e:	ab 81       	ldd	r26, Y+3	; 0x03
 170:	bc 81       	ldd	r27, Y+4	; 0x04
 172:	80 3a       	cpi	r24, 0xA0	; 160
 174:	96 48       	sbci	r25, 0x86	; 134
 176:	a1 40       	sbci	r26, 0x01	; 1
 178:	b1 05       	cpc	r27, r1
 17a:	08 f0       	brcs	.+2      	; 0x17e <L0^A+0x2>

0000017c <L0^A>:
 17c:	a0 cf       	rjmp	.-192    	; 0xbe <.L22>
 17e:	89 81       	ldd	r24, Y+1	; 0x01
 180:	9a 81       	ldd	r25, Y+2	; 0x02
 182:	ab 81       	ldd	r26, Y+3	; 0x03
 184:	bc 81       	ldd	r27, Y+4	; 0x04
 186:	01 96       	adiw	r24, 0x01	; 1
 188:	a1 1d       	adc	r26, r1
 18a:	b1 1d       	adc	r27, r1
 18c:	89 83       	std	Y+1, r24	; 0x01
 18e:	9a 83       	std	Y+2, r25	; 0x02
 190:	ab 83       	std	Y+3, r26	; 0x03
 192:	bc 83       	std	Y+4, r27	; 0x04
 194:	ea cf       	rjmp	.-44     	; 0x16a <.L20>

00000196 <.L11>:
 196:	8d 89       	ldd	r24, Y+21	; 0x15
 198:	9e 89       	ldd	r25, Y+22	; 0x16
 19a:	af 89       	ldd	r26, Y+23	; 0x17
 19c:	b8 8d       	ldd	r27, Y+24	; 0x18
 19e:	01 96       	adiw	r24, 0x01	; 1
 1a0:	a1 1d       	adc	r26, r1
 1a2:	b1 1d       	adc	r27, r1
 1a4:	8d 8b       	std	Y+21, r24	; 0x15
 1a6:	9e 8b       	std	Y+22, r25	; 0x16
 1a8:	af 8b       	std	Y+23, r26	; 0x17
 1aa:	b8 8f       	std	Y+24, r27	; 0x18
 1ac:	8d cf       	rjmp	.-230    	; 0xc8 <.L10>

000001ae <.L13>:
 1ae:	89 89       	ldd	r24, Y+17	; 0x11
 1b0:	9a 89       	ldd	r25, Y+18	; 0x12
 1b2:	ab 89       	ldd	r26, Y+19	; 0x13
 1b4:	bc 89       	ldd	r27, Y+20	; 0x14
 1b6:	01 96       	adiw	r24, 0x01	; 1
 1b8:	a1 1d       	adc	r26, r1
 1ba:	b1 1d       	adc	r27, r1
 1bc:	89 8b       	std	Y+17, r24	; 0x11
 1be:	9a 8b       	std	Y+18, r25	; 0x12
 1c0:	ab 8b       	std	Y+19, r26	; 0x13
 1c2:	bc 8b       	std	Y+20, r27	; 0x14
 1c4:	92 cf       	rjmp	.-220    	; 0xea <.L12>

000001c6 <.L15>:
 1c6:	8d 85       	ldd	r24, Y+13	; 0x0d
 1c8:	9e 85       	ldd	r25, Y+14	; 0x0e
 1ca:	af 85       	ldd	r26, Y+15	; 0x0f
 1cc:	b8 89       	ldd	r27, Y+16	; 0x10
 1ce:	01 96       	adiw	r24, 0x01	; 1
 1d0:	a1 1d       	adc	r26, r1
 1d2:	b1 1d       	adc	r27, r1
 1d4:	8d 87       	std	Y+13, r24	; 0x0d
 1d6:	9e 87       	std	Y+14, r25	; 0x0e
 1d8:	af 87       	std	Y+15, r26	; 0x0f
 1da:	b8 8b       	std	Y+16, r27	; 0x10
 1dc:	95 cf       	rjmp	.-214    	; 0x108 <.L14>

000001de <.L17>:
 1de:	89 85       	ldd	r24, Y+9	; 0x09
 1e0:	9a 85       	ldd	r25, Y+10	; 0x0a
 1e2:	ab 85       	ldd	r26, Y+11	; 0x0b
 1e4:	bc 85       	ldd	r27, Y+12	; 0x0c
 1e6:	01 96       	adiw	r24, 0x01	; 1
 1e8:	a1 1d       	adc	r26, r1
 1ea:	b1 1d       	adc	r27, r1
 1ec:	89 87       	std	Y+9, r24	; 0x09
 1ee:	9a 87       	std	Y+10, r25	; 0x0a
 1f0:	ab 87       	std	Y+11, r26	; 0x0b
 1f2:	bc 87       	std	Y+12, r27	; 0x0c
 1f4:	98 cf       	rjmp	.-208    	; 0x126 <.L16>

000001f6 <.L19>:
 1f6:	8d 81       	ldd	r24, Y+5	; 0x05
 1f8:	9e 81       	ldd	r25, Y+6	; 0x06
 1fa:	af 81       	ldd	r26, Y+7	; 0x07
 1fc:	b8 85       	ldd	r27, Y+8	; 0x08
 1fe:	01 96       	adiw	r24, 0x01	; 1
 200:	a1 1d       	adc	r26, r1
 202:	b1 1d       	adc	r27, r1
 204:	8d 83       	std	Y+5, r24	; 0x05
 206:	9e 83       	std	Y+6, r25	; 0x06
 208:	af 83       	std	Y+7, r26	; 0x07
 20a:	b8 87       	std	Y+8, r27	; 0x08
 20c:	9d cf       	rjmp	.-198    	; 0x148 <.L18>

0000020e <_exit>:
 20e:	f8 94       	cli

00000210 <__stop_program>:
 210:	ff cf       	rjmp	.-2      	; 0x210 <__stop_program>
