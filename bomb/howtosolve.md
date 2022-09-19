# Solution of bomb lab

recording progress.

```
objdump -d bomb > bomb.txt
objdump -s -j .rodata bomb >> bomb.txt
objdump -s -j .data bomb >> bomb.txt
```


## Phase 1

0. Ctrl+F phase_1

```
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	call   401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	call   40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	ret
```

calling `strings_not_equal` with param `%rdi` as input and `%esi` as `0x402400`

1. `strings_not_equal`

`strings_not_equal(x,y)`

(`x` in `%rdi`, `y` in `%rsi`)

The function works as expected.
to examine .rodata section, dump it with
```
objdump -s -j .rodata bomb >> bomb.txt 
```
as stated on the very top.

now just read data in `0x402400` address.

2. The answer
> Border relations with Canada have never been better.

## Phase 2

1. in `phase_2`

%rsi = %rsp
 can do.


call `read_six_numbers` with %rdi as input and %rsi = %rsp (location to store??)


2. in `read_six_numbers`

%rdx = %rsi
%rcx = %rsi + 4
*(%rsp+4) = %rsi + 20
*(%rsp+0) = %rsi + 16
%r9 = %rsi + 12
%r8 = %rsi + 8

... just passing the locations in order. (no 'unscrambling'.)

%rsi = 0x4025c3 // from .rodata, the format string is "%d %d %d %d %d %d"
%rax = 0

3. comparing

if %rsp[0] != 1 explode
%rbx = %rsp + 4
%rbp = %rsp + 24

do{
%eax = *(%rbx-4)
%eax *= 2
if *(%rbx) != %eax explode
%rbx += 4
} while (%rbp != %rbx)

Observing that %rbp is fixed,
%rbx runs from %rsp ~ %rsp + 20, i.e. the values are %rsp[0] ~ %rsp[5].
The only values which avoids explosion are 1 2 4 8 16 32 in turns.

4. the answer
> 1 2 4 8 16 32

## Phase 3

1. in `phase_3`

// %rdi = user input as usual.
%esi = 0x4025cf //from .rodata, the format string is "%d %d"

%rdx = %rsp + 8
%rcx = %rsp + 12

read two unsigned integers, each of which we denote d and c respectively. (for short, we denote %eax = a)
if d > 7 explode
a = d
jmp 402470 + 8 * d. // 400f75

2. flyers
read flyers from .rodata.

d = 0 => 400f7c => a = 0xcf
d = 1 => 400fb9 => a = 0x137
d = 2 => 400f83 => a = 0x2c3
d = 3 => 400f8a => a = 0x100
d = 4 => 400f91 => a = 0x185
d = 5 => 400f98 => a = 0xce
d = 6 => 400f9f => a = 0x2aa
d = 7 => 400fa6 => a = 0x147

if not c == a explode

3. the answer
(all of the answers below are possible)
 can do.

> 0 207  
> 1 311
> 2 707
> 3 256
> 4 389
> 5 206
> 6 682
> 7 327

## Phase 4

1. in `phase_4`
same two number guessing. 

%rdx = %rsp + 8
%rcx = %rsp + 12

value d, c unsigned.
if not d <= 14 explode.

call func4(d, 0, 14).

2. in `func4`

%edi = d, %esi = 0, %edx = 14

%eax = %edx - %esi

%eax = %eax + (%eax >> 31)  // fill with zero. essentially, this is %eax + signbit(%eax).
%eax = %eax >> 1 // fill with sign bit
//so basically this executes %eax /= 2

%ecx = %eax + %esi
if(%ecx <= d) jump 400ff2;
return func4(d, %esi, %ecx - 1) * 2

.400ff2:
%eax = 0
if(%ecx >= d) jump 401007;
return func4(d, %ecx + 1, %edx) * 2 + 1

.401007:
return %eax // return 0

we will try with
func4(7,0,14) = 9
func4(3,0,14) = 2 * func(3,0,6) = 0
func4(8,0,14) = 2 * func(8,8,14) + 1 = 4 * func(8,8,10) + 1 = ... = 1
...

so basically this is like
```
int f(int d, int s, int e){
	int m = s + (e-s)/2;
	if(d<m) return 2 * f(d,s,m-1);
	if(d>m) return 2 * f(d,m+1,e) + 1;
	return 0;
}
```

3. after calling `func4`

a = func4(d,0,14)
if a !=0 explode
if not c == 0 explode

we can find all values d which make func4(d,0,14) != 0
namely d = 0, 1, 3, 7 can do.

4. the answers
(all of the answers below are possible)
> 0 0
> 1 0
> 3 0
> 7 0

## Phase 5

1. in `phase_5`

%rdi = input.
%rbx = input
%rsp[0x18] = (%fs:0x28)
if not string_length(%rdi) == 6 explode

for(%rax=0, %rax !=6, %rax += 1){
  %rdx = input[%rax] & 1111(b)
  %rsp[0x10+%rax] = 0x4024b0[%rdx] 1byte
}
%rsp[0x16] = 0
%rdi = &(%rsp[0x10])
%esi = 0x40245e
if not strings_not_equal(&(%rsp[0x10]), 0x40245e) explode

2. check .rodata

0x4024b0 : m a d u i e r s n f o t v b y l
0x40245e : flyers

to make 'flyers', the last 4bits of the string should be:
9 F E 5 6 7

) / . % & '
9 ? > 5 6 7
I O N E F G
Y _ ^ U V W
i o n e f g
y  ~ u v w

3. the answer
any combination of single char per column would work, including:
> ionefg
> Y_.Uf'

## Phase 6

1. in `phase_6`

read_six_numbers(input, %rsp) // *(%rsp) ~ *(%rsp + 0x14) have input numbers.

%r12 = 0
for(%r13 = %rsp; ; %r13 += 4){
  if not *(%r13) <= 6 explode

  // %r13 = %rsp + 4 * %r12.

  %r12 += 1
  if %r12 == 6 goto 401153; 

  %ebx = %r12
  for(%ebx = %r12; %ebx <= 5; ++%ebx){
    if *(%r13) == *(%rsp + 4 * %ebx) explode
  }
} // six integers must be distinct and in range of 0 ~ 6.

//401153 ~ 40116d
for(%rax = %rsp; %rax != %rsp + 0x18; %rax += 4){
  *(%rax) = 7 - *(%rax)
} // now the numbers are in range 1 ~ 7.

//40116f
for(%rsi = 0; %rsi != 0x18; %rsi += 4){
  %ecx = *(%rsp + %rsi)
  %rdx = 0x6032d0
  for(%eax = 1; %eax != %ecx; ++%eax){
    %rdx = *(%rdx + 8)
  }
  *(%rsp + 2 * %rsi + 0x20) = %rdx
}

oops, we need
```
objdump -s -j .data bomb >> bomb.txt
```
(how can we know that 6032d0 is in .data? run `objdump -x bomb`.)
now, reading the data in 0x6032d0, by the number of %ecx,
1 => 6032d0
2 => 6032e0
3 => 6032f0
4 => 603300
5 => 603310
6 => 603320
7 => 000000


//4011ab
%rdx = *(%rsp + 0x20)
for(%rax = %rsp + 0x28; %rax != %rsp + 0x50; %rax += 8){
  %rcx = %rdx
  *(%rcx + 8) = %rdx = *(%rax)
}

// let g = 0x6032d8 and l = %rsp + 0x20.
// assume that l stores offset instead of addr = offset + g.
// this code reads:
for(i=1;i<6;++i){
  g[ l[i] ] = l[i-1];
}

// additionally, g might be some kind of linked list...

%rbx = *(%rsp + 0x20)
*(%rdx+8) = 0
for(%ebp = 5; %ebp; --%ebp){
  %rax = *(%rbx + 8)
  if not *(%rbx) >= *(%rax) (as 4bytes) explode
  %rbx = %rax
}

// for every 16 bytes, first 8 bytes are data, and the following 8 bytes are next pointer.
// %rax : next. %rbx : previous. next data should be lower than the previous.

read data on 6032d0.

014c
00a8
039c
02b3
01dd
01bb
the right permutation should be 3 -> 4 -> 5 -> 6 -> 1 -> 2
subtracting from 7, 4 -> 3 -> 2 -> 1 -> 6 -> 5

2. the answer
> 4 3 2 1 6 5

## Secret Phase
This phase is hidden in `phase_defused`.

1. reverse engineering `phase_defused`

it seems like it first call sscanf with
%edi = 0x603870
%esi = 0x402619 (`%d %d %s`)
%rdx = %rsp + 8
%rcx = %rsp + 12
%r8 = %rsp + 16

and seems like 0x603870 belongs to .bss

but before that, seems-like global var `num_input_strings` should be set to 6.

the only function which modifies this var is `read_line`...

2. reverse engineering `read_line`

if(skip()) goto 40151f;

if(*infile == *stdin) exit(8);
if(getenv(0x4025f3) != 0) exit(0);
*infile = *stdin;

if(skip()) goto 40151f;
exit(0);

//40151f
%rax = num_input_strings;
%rsi = 0x603780 + %rax * 80;
%rdi = %rsi;
%eax = 0;
%rcx = strlen(%rsi); // binary version of strlen.
if(%ecx > 0x4e) explode; //Error: input line too long.

%ecx -= 1;
*(0x603780 + %rax * 80 + %rcx) = 0;
num_input_strings += 1;
return %rsi;

// 0x603780 = input_strings. which means buffer size 80?

and in `skip` function the actual `fgets` is called.

3. reverse engineering `skip`

%rdi = 0x603780 + %rax * 80;
fgets(%rdi, 80, *infile); //for simplicity.

4. reverse engineering `phase_defused` (again)

Come to think about it, this `num_input_strings` is set to 6 iff you passed the phase 6. This is totally natural...

now the address 0x603870 must be set with %d %d %s
and the string must match with 0x402622 = "DrEvil".
Once the conditions are met, we can enter the secret phase.

by the way, the buffer location of 0x603780 + %rax * 80 is
 - 1st phase: 0x603780
 - 2nd phase: 0x6037d0
 - 3rd phase: 0x603820
 - 4th phase: 0x603870

In conclusion, updating 4th input into "0 0 DrEvil" leads us to the secret phase.

5. in `secret_phase`

// first it read the value (start ptr in %rax)

%rax = strtol(input, NULL, 10); //str to long int.
if not %rax -1 <= 1000 explode;
if not fun7(0x6030f0, %rax) == 2 explode;

6. in `fun7`

if %rdi == 0 return -1;
if (*(%rdi) > %esi) return 2 * fun7(%rdi+8, %esi);
if (*(%rdi) == %esi ) return 0;
return 2 * fun7(%rdi+16, %esi) + 1;

this is a binary tree -- %rdi as value, %rdi + 8 as left, %rdi + 16 as right.
the tree structure is

(6030f0) - value 0x24, L(603110), R(603130)
(603110) - value 0x08, L(603190), R(603150)
(603130) - value 0x32, L(603170), R(6031b0)
(603150) - value 0x16, L(603270), R(603230)
(603170) - value 0x2d, L(6031d0), R(603290)
(603190) - value 0x06, L(6031f0), R(603250)
(6031b0) - value 0x6b, L(603210), R(6032b0)
(6031d0) - value 0x28
(6031f0) - value 0x01
(603210) - value 0x63
...
(6032b0) - value 0x3e9

starts from 6030f0.
less than 0x24 -- take L(603110)
greater than 0x08 -- take R(603150)
less than or equal to 0x16 -- if not 22, take L(603270)
less than or equal to 0x14 -- if not 20, there is no L.

7. the answer
both are correct:
> 20
> 22
