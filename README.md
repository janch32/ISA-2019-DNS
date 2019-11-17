# DNS resolver
Projekt do předmětu ISA 2019. Program pro dotazování DNS serverů. 

Zdrojový kód je psaný v jazyce C++11 a využívá standardní knihovny jazyka C a C++. Seznam použitých knihoven je v dokumentaci.

## Překlad a testování
### Požadavky
Před samotnou kompilací je nutné, aby na systému byly nainstalované následující nástroje
 * GNU verze programu Make
	 * Na Linuxu jako standardní program `make`, v jiných systémech (např. OpenBSD) může být tento program pod názvem `gmake`
 * C compiler (Makefile využívá pro kompilaci program v proměnné `CC`)
 * Node.JS ve verzi 13.x nebo vyšší (nutné jen pro testování)

### Make příkazy
Makefile je psaný pro **GNU Make**. Výchozí příkaz je `all`.
Pokud chcete přeložit projekt příkazem `debug` a je již přeložen příkazem `all` (a naopak), je nutné před tím spustit čistící příkaz `clean`.

#### `all`
Přeložení programu pro běžné používání. Po úspěšné kompilaci se vytvoří v kořenové složce projektu tento program pod názvem `dns`.

#### `debug`
Zkompiluje program pro vývoj. Na rozdíl od běžného překladu výsledný program bude obsahovat pomocná data pro `gdb` a při spuštění vypíše program na standardní výstup odeslaná a přijatá data v hexa formátu.

#### `clean`
Odstraní ze složky projektu soubory vytvořené kompilací.

#### `test`
*Poznámka: Tento příkaz vyžaduje Node.JS ve verzi >= 13.x, který musí být dostupný v proměnné PATH pod názvem `node`.*

Spuštění automatických testů. Před spuštěním je proveden překlad programu v debug módu. Testování probíhá stylem blackbox a na standardní výstup je postupně vypisován formátovaný text o průběhu a výsledcích testů. 

Skript vytváří dočasný UDP server pro simulaci DNS serveru, který naslouchá lokálně na portu 32323. Proto je potřeba ověřit, zda na tomto portu žádný jiný program nenaslouchá a popřípadě tento port změnit v souboru `./test/udp_server.js`.

## Spuštění programu
### Použití
`dns [-rx6tmc] -s server [-p port] address`
### Parametry dotazu
Pokud žádný z těchto parametrů není specifikován, použije se dotaz typu A. Typy dotazu nelze kombinovat (například kombinace `-6x` není povolena)
 * `-6` Dotaz typu AAAA
 * `-t` Dotaz typu TXT
 * `-m` Dotaz typu MX
 * `-c` Dotaz typu CNAME
 * `-x` Dotaz typu PTR
### Ostatní parametry
 * `-r` Dotaz provést rekurzivně (pokud je podporováno serverem)
 * `-s server` Adresa dotazovaného DNS serveru (povinné)
 * `-p port` Port na kterém server naslouchá (výchozí = 53)
 * `address` Dotazovaná adresa. (povinné)
	 * Adresa je ve formě doménového jména (př.: `google.com`)
	 * Pokud je dotaz typu PTR (parametr `-x`), musí být adresa zadána jako IPv4 nebo IPv6

## Příklady použití
### Dotaz na IPv4 (A záznamy) domény
```
$ dns -rs8.8.8.8 www.fit.vutbr.cz
Authoritative: No, Recursive: Yes, Truncated: No
Question section (1)
  www.fit.vutbr.cz., A, IN
Answer section (1)
  www.fit.vutbr.cz., A, IN, 14144, 147.229.9.23
Authority section (0)
Additional section (0)
```

### Reverzní dotaz na IP adresu
```
$ dns -rx -s8.8.8.8 147.229.9.23
Authoritative: No, Recursive: Yes, Truncated: No
Question section (1)
  23.9.229.147.in-addr.arpa., PTR, IN
Answer section (1)
  23.9.229.147.in-addr.arpa., PTR, IN, 10581, www.fit.vutbr.cz.
Authority section (0)
Additional section (0)
```

### Dotaz na MX záznamy domény
```
$ dns -rm -s8.8.8.8 seznam.cz
Authoritative: No, Recursive: Yes, Truncated: No
Question section (1)
  seznam.cz., MX, IN
Answer section (2)
  seznam.cz., MX, IN, 76, 10 mx1.seznam.cz.
  seznam.cz., MX, IN, 76, 20 mx2.seznam.cz.
Authority section (0)
Additional section (0)
```
### Dotaz na IPv6 (AAAA) záznamy domény
```
$ dns -r6 -s8.8.8.8 www.ietf.org
Authoritative: No, Recursive: Yes, Truncated: No
Question section (1)
  www.ietf.org., AAAA, IN
Answer section (3)
  www.ietf.org., CNAME, IN, 1433, www.ietf.org.cdn.cloudflare.net.
  www.ietf.org.cdn.cloudflare.net., AAAA, IN, 123, 2606:4700:10::6814:155
  www.ietf.org.cdn.cloudflare.net., AAAA, IN, 123, 2606:4700:10::6814:55
Authority section (0)
Additional section (0)
```

## Rozšíření
Navíc oproti zadání je program rozšířen o parametry `-t`, `-m` a `-c`, které umožňují použít další typy dotazů. Popis, jak fungují je v sekci "Spuštění programu".
