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

La première étape est donner une IP a `eth0` de R1 à l'aide de DHCP:

```
ssh R1 dhclient -v eth0
```

Et une IP fixe à `eth1`:

```
ssh R1 ip address add 10.0.0.2/24 dev eth1
ssh R1 ip link set eth1 addr 00:00:00:00:00:1
```

Ensuite, on configure `H1` pour qu'il ai sa propre ip:

```
ssh H1 ip address add 10.0.0.1/24 dev eth0
ssh H1 ip link set eth0 addr 00:00:00:00:00:2
```

Finalement on démarre toute les inferfaces:

```
ssh R1 ip link set dev eth1 up
ssh R1 ip link set dev eth0 up
ssh H1 ip link set dev eth0 up
```

Puis, nous configurons `H1` pour utiliser `R1` comme passerelle par défaut:

```
ssh H1 ip route add default via 10.0.0.2 dev eth0
```

Comme R1 est un routeur, il forwarde des paquets et devra avoir /proc/sys/net/ipv4/ip_forward posé à 1.

```
ssh R1 sysctl net.ipv4.ip_forward=1
```

R1 implémentera une passerelle NAT à l’aide de nftables pour cela, il faut exécuter le script nftables suivant sur R1.

`nat_R1`

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

```
parent_path=$(cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P)
scp "$parent_path/nat_R1" R1:/tmp/nat_R1
ssh R1 chmod +x /tmp/nat_R1
ssh R1 /tmp/nat_R1
```

Et comme ça, H1 et R1 ont bien accès à internet et peux maintenant installer tcpdump.

```
ssh R1 apt install -y tcpdump
ssh H1 apt install -y tcpdump
```

# Annexes

## 1A.sh

```bash
#!/bin/bash
set -e

echo "L’IP obtenue par R1 coté eth0 sera obtenue par DHCP avec la commande dhclient -v eth0,"
echo "car un serveur DHCP est disponible coté nat0"
ssh R1 dhclient -v eth0

echo "L’IP de R1 coté eth1 et de H1 eth0 sera configurée statiquement avec iproute2,"
echo "dans le segment IP 10.0.0.0/24. La configuration d’une IP avec iproute est disponible"
echo "dans la documentation 2"
ssh R1 ip address add 10.0.0.2/24 dev eth1
ssh R1 ip link set eth1 addr 00:00:00:00:00:1
ssh R1 ip link set dev eth1 up
ssh R1 ip link set dev eth0 up

ssh H1 ip address add 10.0.0.1/24 dev eth0
ssh H1 ip link set eth0 addr 00:00:00:00:00:2
ssh H1 ip link set dev eth0 up

echo "La route par défaut de H1 passera par R1, référez vous à la documentation d’iproute"
echo "pour installer une route par défaut 1"
ssh H1 ip route add default via 10.0.0.2 dev eth0

echo "Comme R1 est un routeur, il forwarde des paquets et devra avoir"
echo "/proc/sys/net/ipv4/ip_forward posé à 1."
ssh R1 sysctl net.ipv4.ip_forward=1

echo "R1 implémentera une passerelle NAT à l’aide de nftables pour cela, il faut"
echo "exécuter le script nftables suivant sur R1"
parent_path=$(cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P)
scp "$parent_path/nat_R1" R1:/tmp/nat_R1
ssh R1 chmod +x /tmp/nat_R1
ssh R1 /tmp/nat_R1

echo "waiting for network to be up..."
sleep 5

echo "Installation de tcpdump sur H1 et R1 avec la commande apt install -y tcpdump."
ssh R1 apt install -y tcpdump
ssh H1 apt install -y tcpdump
```

## 1B.sh

```bash
#!/bin/bash
set -e

ssh H1 ping -c 4 1.1.1.1
ssh H1 ping -c 4 10.0.0.2
ssh H1 traceroute -n 1.1.1.1

ssh R1 ping -c 4 1.1.1.1
ssh R1 ping -c 4 10.0.0.1
```

## 2A.sh

```bash
#!/bin/bash
set -e

echo "Create a new network namespace named ns2:"
ssh R1 ip netns add ns2

echo "Create veth0 and veth1 virtual Ethernet devices:"
echo "also rename to have the same name as in the other namespace"
ssh R1 ip link add veth0 type veth peer name veth1
ssh R1 ip link set veth1 netns ns2
ssh R1 ip netns exec ns2 ip link set veth1 name veth0
ssh R1 ip link add veth1 type veth peer name veth2
ssh R1 ip link set veth2 netns ns2
ssh R1 ip netns exec ns2 ip link set veth2 name veth1

echo "Assign IP addresses to veth0 and veth1:"
ssh R1 ip address add 192.168.0.1/24 dev veth0
ssh R1 ip netns exec ns2 ip address add 192.168.0.2/24 dev veth0
ssh R1 ip address add 192.168.1.1/24 dev veth1
ssh R1 ip netns exec ns2 ip address add 192.168.1.2/24 dev veth1

echo "Enable both interfaces:"
ssh R1 ip link set veth0 up
ssh R1 ip netns exec ns2 ip link set veth0 up
ssh R1 ip link set veth1 up
ssh R1 ip netns exec ns2 ip link set veth1 up

echo "-- copy pasted from https://hepia.infolibre.ch/VRES-2022-2023/labos/virtualisation_de_pile_tcp_ip.html --"
echo "Comme le trafic transite par un routeur virtuel présent sur l’hôte R1,"
echo "il faut pouvoir distinguer le trafic qui provient de l’hôte du trafic"
echo "qui provient de R2 au niveau de la destination par défaut. Pour cela"
echo "nous utiliserons le policy routing et ip rule 3 de la manière suivante"
echo "sur R1:"
echo "Effacement de la route par défaut obtenue par dhclient."
ssh R1 ip route delete default
echo "Redirection des paquets via l'interface veth qui mène à R2."
ssh R1 ip route add default via 192.168.1.2 dev veth1
echo "Utilisation du policy routing pour passer par eth0 si le trafic vient de R2."
echo "172.21.1.1 est la passerelle obtenue par DHCP sur le Nuage Internet."
ssh R1 "echo 100 custom >> /etc/iproute2/rt_tables"
ssh R1 ip rule add iif veth0 table custom
ssh R1 ip route add default via 172.21.1.1 table custom
echo "-- end of copy pasted content --"

echo "Comme R2 est un routeur, il forwarde des paquets et devra avoir"
echo "/proc/sys/net/ipv4/ip_forward posé à 1."
ssh R1 ip netns exec ns2 sysctl net.ipv4.ip_forward=1

echo "R1 implémentera une passerelle NAT à l’aide de nftables pour cela, il faut"
echo "exécuter le script nftables suivant sur R2"
parent_path=$(cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P)
scp "$parent_path/nat_R2" R1:/tmp/nat_R2
ssh R1 chmod +x /tmp/nat_R2
ssh R1 ip netns exec ns2 /tmp/nat_R2

echo "Adding default routes to R2"
ssh R1 ip netns exec ns2 ip route add default via 192.168.0.1 dev veth0
ssh R1 ip netns exec ns2 ip route add 10.0.0.0/24 via 192.168.1.1 dev veth1
```
