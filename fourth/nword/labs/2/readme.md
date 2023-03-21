---
pdf_options:
  format: a4
  margin: 30mm 20mm
  printBackground: true
  headerTemplate: |-
    <style>
      section {
        margin: 0 auto;
        font-family: system-ui;
        font-size: 11px;
      }
    </style>
    <section>
      <span>Antoine Sutter</span>
    </section>
  footerTemplate: |-
    <section>
      <div>
        Page <span class="pageNumber"></span>
        / <span class="totalPages"></span>
      </div>
    </section>
---

# Labo 2 : Equipements réseaux virtuels basé sur les namespaces Linux

## A - Préparation du routeur hôte

Afin de preparer le labo, un script bash a été créé.

Premièrement, on assigne une addresse ip sur `eth0` à `R1` avec DHPC:

```
ssh R1 dhclient -v eth0
```

Ensuite on configure les interfaces `eth0` et `eth1` de `R1`. Il est important de up les interfaces.

```
ssh R1 ip address add 10.0.0.2/24 dev eth1
ssh R1 ip link set eth1 addr 00:00:00:00:00:1
ssh R1 ip link set dev eth1 up
ssh R1 ip link set dev eth0 up
```

Puis le configure `H1` pour qu'il ai sa propre ip:

```
ssh H1 ip address add 10.0.0.1/24 dev eth0
ssh H1 ip link set eth0 addr 00:00:00:00:00:2
ssh H1 ip link set dev eth0 up
```

et pour utiliser `R1` comme passerelle par défaut.

```
ssh H1 ip route add default via 10.0.0.2 dev eth0
```

Comme dans le TP précédent, il faut activer le forwarding de paquets sur `R1`:

```
ssh R1 sysctl net.ipv4.ip_forward=1
```

Pour implémenter une passerelle NAT sur `R1`, le script suivant est utilisé:

```
#!/usr/sbin/nft -f

flush ruleset

table ip nat {
    chain masq {
        type nat hook postrouting priority 100;
        oifname "eth0" counter masquerade
    }
}
```

Ce fichier est à côté du fichier bash et est copié sur `R1` avec la scp:

```
parent_path=$(cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P)
scp "$parent_path/nat" R1:/tmp/nat
ssh R1 chmod +x /tmp/nat
ssh R1 /tmp/nat
```

Puis finalement, maintenant que `R1` et `H1` sont configurés, on peut installer `tcpdump` sur les deux machines.

```
ssh R1 apt install -y tcpdump
ssh H1 apt install -y tcpdump
```

## B - Vérification du bon fonctionnement

`H1` peut ping vers internet:

```
➜ ssh H1 ping -c 4 1.1.1.1
Warning: Permanently added '[100.64.0.8]:2015' (ED25519) to the list of known hosts.
PING 1.1.1.1 (1.1.1.1) 56(84) bytes of data.
64 bytes from 1.1.1.1: icmp_seq=1 ttl=54 time=4.31 ms
64 bytes from 1.1.1.1: icmp_seq=2 ttl=54 time=3.49 ms
64 bytes from 1.1.1.1: icmp_seq=3 ttl=54 time=3.56 ms
64 bytes from 1.1.1.1: icmp_seq=4 ttl=54 time=4.21 ms

--- 1.1.1.1 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 3005ms
rtt min/avg/max/mdev = 3.489/3.893/4.312/0.370 ms
```

On peut également vérifier que `H1` passe bien par `R1` pour accéder à internet:

```
➜ ssh H1 traceroute -n 1.1.1.1
Warning: Permanently added '[100.64.0.8]:2015' (ED25519) to the list of known hosts.
traceroute to 1.1.1.1 (1.1.1.1), 30 hops max, 60 byte packets
 1  10.0.0.2  0.642 ms  0.500 ms  0.510 ms
 2  172.21.1.1  1.023 ms  1.057 ms  1.126 ms
 3  172.17.0.1  1.562 ms  1.350 ms  1.296 ms
 4  10.10.0.1  1.479 ms  1.456 ms  1.401 ms
 5  10.35.200.65  2.529 ms  2.597 ms  2.596 ms
 6  195.176.1.12  3.830 ms  5.078 ms  2.582 ms
 7  130.59.38.193  9.605 ms  5.597 ms  5.400 ms
 8  130.59.36.70  3.015 ms  5.251 ms  2.983 ms
 9  192.65.185.228  6.485 ms  4.396 ms  3.242 ms
10  1.1.1.1  3.067 ms  3.570 ms  2.757 ms
```

Comme on peut le voir sur l'output de la commande `traceroute`, le premier hop est `R1` avec l'ip `10.0.0.2` que nous avons configuré précédemment.

# 2 - Routeur virtuel routé

## A - Préparation
