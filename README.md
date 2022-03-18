# picup

`Picup` adds the option to Cobalt Strike to send shellcode output to the console. 

First, the aggressor script injects the shellcode with `shinject`. The shellcode than starts a Named Pipe to write its output to it. Than the BOF is loaded and conntects to the Named Pipe. There it reads all the data an print it to the console by using `BeaconPrintf`. 

Of course, the Beacon object files (BOFs) are suitable for this and also far better known and supported, but in my case I wanted to run my own shellcode to get better at C and to learn the concept of position indipentend code. Maybe there are even some situations where shellcode is the better choice. 

There are some general advandatges from shellcode to BOFs like the indipendence from C2 (even most C2 have COFF-Loaders integrated) or run it in your own loader C2.



# Usage

## How to compile


```bash
nasm -f win64 adjuststack.asm -o adjuststack.o
nasm -f win64 chkstk_ms.asm -o chkstk_ms.o
x86_64-w64-mingw32-gcc dnscache.c -Wall -m64 -ffunction-sections -fno-asynchronous-unwind-tables -nostdlib -fno-ident -O2 -c -o dnscache.o -Wl,-Tlinker.ld,--no-seh
x86_64-w64-mingw32-ld -s adjuststack.o chkstk_ms.o dnscache.o -o dnscache.exe
for i in $(objdump -d dnscache.exe |grep "^ " | cut -f2); do echo -e -n "\x$i"; done > dnscache.bin
```
or run

```bash
bash compile.sh
```

The BOF.

```
 x86_64-w64-mingw32-gcc -c piper.x64.c -o piper.x64.o
```

## Use it with Cobalt Strike
```bash
# Clone the repository to your clients Cobalt Strike folder 
git clone https://github.com/svnsyn/picup
cd picup
bash compile.sh 
```

```
beacon> picup dnscache
```


# Example

```
beacon> picup dnscache
[*] Waiting for module named pipe.
[*] Injecting Module.
[+] host called home, sent: 1106 bytes
[*] Tasked beacon to inject C:\Users\rene\Desktop\cobaltstrike4.5\minject\dnscache.bin into 5088 (x64)
[+] received output:
[+] Pipe closed by module, GLE=233.

[+] received output:

 www.gstatic.com
 ----------------------------------------
 A Record: 104.26.3.5
 
 example.org
 ----------------------------------------
 A Record: 93.184.216.34

 www.googletagmanager.com
 ----------------------------------------
 A Record: 160.215.147.0
 A Record: 142.250.181.232

 gbc4.fr.eu.criteo.com
 ----------------------------------------
 A Record: 185.235.84.130
 A Record: 185.235.84.116
 A Record: 185.235.84.128
 A Record: 185.235.84.134
 A Record: 185.235.84.118
 A Record: 185.235.84.115
 pr-bh.ybp.yahoo.com
 ----------------------------------------
 adservice.google.com
 ----------------------------------------
 A Record: 172.217.23.98
 ade.googlesyndication.com
 [...]

[+] host called home, sent: 3615 bytes
```

------------

# Modules 

## Dnscache PIC Module

Reads the dns cache of the system like `ipconfig /displaydns` would. No network calls are made since the [DnsQuery_W](https://docs.microsoft.com/en-us/windows/win32/api/windns/nf-windns-dnsquery_w) are executed with the query option [DNS_QUERY_NO_WIRE_QUERY](https://docs.microsoft.com/en-us/windows/win32/dns/dns-constants). Resolves DNS-Record to IP address if the entry exists.

------------

# Todo

- Random or common names for Named Pipes.
- Just use the aggressor script to read from the Named Pipe (like here [repo](https://github.com/rxwx/cs-rdll-ipc-example)).
- ...

# References


- [@Hasherezade](https://twitter.com/hasherezade): This [paper](https://vxug.fakedoma.in/papers/VXUG/Exclusive/FromaCprojectthroughassemblytoshellcodeHasherezade.pdf) 
- [@ParanoidNinja](https://twitter.com/NinjaParanoid): This [code](https://github.com/paranoidninja/PIC-Get-Privileges) and this great [tutorial](https://bruteratel.com/research/feature-update/2021/01/30/OBJEXEC/): Used  `adjuststack.asm` and `addresshunter.h` from it. 
- [Beacon Object Files](https://hstechdocs.helpsystems.com/manuals/cobaltstrike/current/userguide/content/topics/beacon-object-files_main.htm)
- [Aggressor Scripts](https://trial.cobaltstrike.com/aggressor-script/index.html).
- [@csandker](https://twitter.com/0xcsandker): One of the best [blosposts](https://csandker.io/2021/01/10/Offensive-Windows-IPC-1-NamedPipes.html) about Named Pipes I was able to find (with security perspective). 
- [@Bufflevoerflow](https://twitter.com/buffaloverflow): This [repo](https://github.com/rxwx/cs-rdll-ipc-example) was my foundation.
- [@MrUn1k0d3r](https://twitter.com/MrUn1k0d3r/): Really awesome [Patreon](https://www.patreon.com/MrUn1k0d3r). 
- [@thefLink](https://github.com/codewhitesec/HandleKatz): On of the best sources for learning C (PIC-Style and in general) and I used `chkstk_ms.asm` from it. 
- [React OS](https://reactos.org/): Implementation of [ipconfig](https://doxygen.reactos.org/d3/dae/ipconfig_8c_source.html)



