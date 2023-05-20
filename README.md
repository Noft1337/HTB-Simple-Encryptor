# HTB-Simple-Encryptor
This is the solution to the HTB reversing challange - Simple Encryptor 
  `flag.enc` - the challange flag after being encrypted
  `reversed.c` - The source code of the encryptor, which I have reversed and retyped to better understand the source code.
  `main.c` - The program that should decrypt `flag.enc` to human readable characters.
## Encrypting
The encryption program firstly reads a flag which is stored on a file named `flag.txt`, encrypts it and stores it on `flag.enc`.
The encryption process is being done by first generating a `seed` for the `srand()` function, the seed is the `EPOCH Time` of when the program is being ran.
Then iterating the whole `flag.txt` file contents and for each iteration generating 2 random 8 bit integers we'll call `rand1` and `rand2` for the sake of this explanation.
Then in every iteration we have a char of that iteration, let's call it `current_char` we run a `xor` on the `current_char` with `rand1` which is of `unsigned char` data type, meaning it can be any value between `0 and 255`.
After that the encryptor runs an `or` function on `current_char` (after the `xor`).
It `ors` between `current_char << rand2` and `current_char >> 8 - rand2` which is actually a bit swap because `rand2` is a vaule between `1 and 7`(including both edges).
I have simplified it to assembly code because it's the best way (In my opinion) to visualize it.
```ASSEMBLY
MOV EAX, current_char
XOR EAX, rand1
MOV EBX, EAX
SHL EAX, rand2
SHR EBX, 8 - rand2
OR EAX, EBX
```

## Decrypting 
Decryption is simple but it depends on having the right `seed` in order to get the random numbers that were generated.
In `reverse.c` we can see that the seed is being written to `flag.enc` in the first 4 bytes. So, that's how we get the seed.
The rest of the decryption is pretty simple, just generate random numbers using the `seed` we have found and then \`unshift\` and \`unxor\` every char.
