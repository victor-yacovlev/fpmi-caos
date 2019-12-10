# Encryption using OpenSSL / LibreSSL

## Linux Encryption Basics

Cryptography in Linux, as in many other UNIX-like systems, is implemented using the `openssl` package or the fork of` libressl` compatible with it.

The package provides:
 * command `openssl` to perform operations on the command line
 * library `libcrypto` with the implementation of encryption algorithms
 * library `libssl` with the implementation of interaction via SSL and TLS.

### Calculating Hash Values

Commands:
 * `openssl md5`
 * `openssl sha256`
 * `openssl sha512`

calculate the hash value for the specified file and output it in a readable form to the standard output stream. The optional option `-binary` indicates the output in binary format. If no filename is specified, a hash value is calculated for the data from the standard input stream.

### Symmetric Encryption

Command:

```
openssl enc -CHIPHER -in FILENAME -out EXIT
```

Performs encryption *with a symmetric key*, which means with some “password”, that is the same for both encryption and the reverse decryption operation.

A complete list of supported ciphers is displayed with the `openssl enc -ciphers` command. Most commonly used:
 * `des` is a rather old algorithm using a 56-bit key;
 * `aes256` or` aes-256-cbc` - more reliable and fast enough;
 * `base64` - no encryption (key is not required); A convenient way to convert binary files to text representation and vice versa.

The `-d` option means inverse conversion, i.e. *decryption*. The `-base64` option implies that the encrypted data is additionally converted to Base64 encoding, for example, to transfer data in the form of text.

After running the command, a password and its confirmation will be requested. In the case when you need to automate the launch of the command, the `-pass` option is used, after which it is transmitted how the password is set:
 * `pass: PASSWORD` - the password is set in plain text as an argument to the command line; terribly unsafe;
 * `env: VARIABLE` - the password is set by a specific environment variable; a little better, but can be figured out through `/ proc /.../ environ`;
 * `file: NAME` - the password is taken from the file;
 * `fd: NUMBER` - the password is taken from the file descriptor with the specified number; used at startup via `fork` +` exec`.

Since symmetric encryption algorithms imply the use of a fixed-size key, a text password of arbitrary length is pre-converted using a hash function. By default, SHA-256 is used, but this can be set using the option `-md ALGORITHM`.

In addition to the password, the key also includes another component - *salt* of 8 bytes in size, which is stored in the encrypted file itself. This value is randomly generated, but for reproducibility, the result can be explicitly set using the `-S HEX` option, where` HEX` is an eight-byte value in hexadecimal notation.


### Encryption using a key pair

The standard algorithm for encryption using a key pair is RSA.

Key generation is performed by the command:
```
openssl genrsa -out FILE BIT
```

If the name of the output file is not specified, then the key in text format will be saved to the standard output stream. Typically, RSA keys are stored in files with a suffix of the name `.pem`.

Bit depth determines the strength of the key, by default - 2048 bits.

Since the private key must be stored somewhere, and in a safe way, it is considered good practice to store it in encrypted form, encryption with a symmetric key is used for this:
```
openssl genrsa -aes256 -passout PASSWORD OPTIONS
```

When using an encrypted private key, it will be necessary to indicate the password specified during its creation each time.

The extraction of the public key from the private is carried out by the command:
```
openssl rsa -in PRIVATE_KEY -out PUBLIC_KEY -pubout
```

If encryption was used when creating the key pair, you must enter a password or set it using `-passin`.

Public Key Encryption:
```
openssl rsautl -encrypt -pubin -inkey PUBLIC_KEY -in FILE -out EXIT
```

The reverse operation using the private key:
```
openssl rsautl -decrypt -inkey PRIVATE_KEY -in FILE -out EXIT
```

A limitation of the RSA algorithm is that the size of the encrypted data cannot exceed the size of the key. You can deal with this in the following ways:
 1. Divide the source data into blocks of 2 or 4 KB in size and encrypt them individually
 2. Randomly generate a one-time *session key*, which will be used in conjunction with a symmetric encryption algorithm, but will itself be encrypted using RSA.

```
# Generate a random key 30 bytes long and save
# its Base64 textual representation in the $ KEY variable
KEY = `openssl rand -base64 30`


# We encrypt the symmetric key using the RSA public key
echo $ KEY | openssl rsautl -encrypt -pubin \
                           -inkey public.pem \
                           -out symm_key_encrypted
```


## Library

Here is an example of library usage:
```c
#include <openssl/sha.h>

int main(int argc, char *argv[])
{
    SHA512_CTX context;
    SHA512_Init(&context);
    // hash data
    SHA512_Final(hash, &context);
}
```