# Passman

This is a simple record-less CLI password manager in C++ that uses Argon2id KDF to generate the password.

## Building

```bash
make all
```

This will create a pman binary for you to use.

To remove the created executable use:

```bash
make clean
```

### Requirements

Requires [argon2.h](https://github.com/P-H-C/phc-winner-argon2/blob/master/include/argon2.h) in the same directory, as well as libargon2.a.
You can follow the instructions on [Argon2's github](https://github.com/P-H-C/phc-winner-argon2) to generate the library for your system.

## Usage

```bash
./pman [-i iterations] [-m memory] [-p parallelism] example.com "john doe" "master password123" 1
```

`-i iterations` - default 8  
This is the number of iterations the algorithm is run.

`-m memory` - default 18 (256MB)  
This is the memory required to efficiently use the algorithm, you should aim to maximize this for your system.  
To use 64MB, you would do `-m 16`, megabytes used = `2^m`.

`-p parallelism` - default 8  
The number of threads to run the algorithm on, you should set this to the number of threads you have.

`example.com`  
The url of the website or name of the service you wish to create a password for.

`"john doe"`  
The account identifier, this can be the account's username or any identifier you prefer.

`"master password123"`  
This is your master password that you will use for any service/account to generate their passwords. You can read [this](https://bitwarden.com/blog/picking-the-right-password-for-your-password-manager/) article on how to pick your master password.

`1`  
This is the number of passwords resets you have done for your account. Since this password manager does not save any passwords, you need a way to be able to generate a new password for the same service/account, to do this you can change this number each time you want a new password.

## Examples

```bash
./pman github "Alice" "naktyldnyh#1970s" 0
```

Output

```bash
67243093b8a20a028b8f27eedce86366
```

---

```bash
./pman -i 4 -m 16 -p 6 example.com "Bob" "my secur3 p4s!word" 1
```

Output

```bash
39fb2657ff1a043ef93a9e628c985299
```

## License

[MIT](https://choosealicense.com/licenses/mit/)
