# Teltonika_Praktika
Programa, kuri nustatytų interneto duomenų parsisiuntimo ir išsiuntimo greitį. Programa gali būti paleista pilnai automatizuotai arba pasirenkant norimą funkciją.

•	Duomenų parsiuntimo greičio nustatymo testo pavyzdys:

Naudojama komanda:

  ./speedtest -d -s (server)


Atliktas testinis variantas:

  ./speedtest -d -s speedtest.tele2.net
  
Rezultatas:

    Atliekamas atsiuntimo testas...
    Apskaiciuotas greitis: 37.07 Mbps

    ---------- gauti rezultatai ----------
	
    Naudotojo vietove: Nerasta
    Serverio atsiuntimo url: http://speedtest.tele2.net/1MB.zip
    Serverio siuntimo url:
    Atsiuntimo greitis: 37.07 Mbps
    Siuntimo greitis: 0.00 Mbps

•	Duomenų išsiuntimo greičio nustatymo testo pavyzdys:
Naudojama komanda:

  ./speedtest -u -s (server)

Atliktas testinis variantas:

  ./speedtest -u -s speedtest.tele2.net

Rezultatas:

    Atliekamas siuntimo testas...

    ---------- gauti rezultatai ----------
    Naudotojo vietove: Nerasta
    Serverio atsiuntimo url:
    Serverio siuntimo url: http://httpbin.org/post
    Atsiuntimo greitis: 0.00 Mbps
    Siuntimo greitis: 14.86 Mbps

•	Atlikti vietovės nustatymą:
Naudojama komanda:
  ./speedtest -l

Rezultatas:

    Nustatoma vieta
    Vietove: Lithuania

    ---------- gauti rezultatai ----------
    Naudotojo vietove: Lithuania
    Serverio atsiuntimo url:
    Serverio siuntimo url:
    Atsiuntimo greitis: 0.00 Mbps
    Siuntimo greitis: 0.00 Mbps

•	Atlikti geriausio serverio pagal vietovę nustatymą:

  Naudojama komanda:
	
  ./speedtest -a

  Rezultatas:

  	Nustatoma vieta
  	Vietove: Lithuania
  	Ieskomas geriausias serveris pagal vietove...
  	Rastas serveris salyje Lithuania: speedtest.litnet.lt:8080
  	Tikrinamas failas: speedtest/random2000x2000.jpg
  	Tikrinamas failas: speedtest/random1000x1000.jpg
  	Tikrinamas failas: 10MB.zip
  	Tikrinamas failas: 5MB.zip
  	Tikrinamas failas: 1MB.zip
  	Tikrinamas failas: 100MB.bin
  	Tikrinamas failas: 10MB.bin
  	Tikrinamas failas: 1MB.bin
  	Serveris rastas, bet failai netinka. Bandome kitą.
  	...
  	...
  	...
  	Nerasta tinkamu serveriu, naudojamas testinis variantas
  	Pasirinktas serveris:
  	Download URL: http://speedtest.tele2.net/10MB.zip
  	Upload URL: http://httpbin.org/post
  	Atliekamas atsiuntimo testas...
  	Apskaiciuotas greitis: 160.01 Mbps
  	Atliekamas siuntimo testas...

  	---------- gauti rezultatai ----------
  	Naudotojo vietove: Lithuania
  	Serverio atsiuntimo url: http://speedtest.tele2.net/10MB.zip
  	Serverio siuntimo url: http://httpbin.org/post
  	Atsiuntimo greitis: 160.01 Mbps
  	Siuntimo greitis: 15.57 Mbps
