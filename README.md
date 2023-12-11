# Interface Contrained Routing: Break Traffic Loop

## Background
If a program listens a tun interface and sets the tun interface as default route destination. Traffic will then be routed to the tun device.

The problem is how does the tun program access the outside world? How to break the traffic loop?

## Solution
Bind socket to interface (*not address*) before conntect/send.  

## Testing
### build
```sh
gcc bound-socket.c -o bound-socket
```

### unbound socket: follow default routing rules
```sh
./bound-socket unbound & sleep 1 && ss -tupan | grep bound
```

Output:
```text
[1] 18373
tcp   ESTAB     0      0                  172.19.0.1:53282      114.114.114.114:53    users:(("bound-socket",pid=18373,fd=3))
```

The traffic enters the tun interface according to the default routing rules.  
Output from tun program:

```text
-0500 2023-12-11 03:22:51 INFO [3235595864 0ms] inbound/tun[tun-in]: inbound connection from 172.19.0.1:53282
-0500 2023-12-11 03:22:51 INFO [3235595864 0ms] inbound/tun[tun-in]: inbound connection to 114.114.114.114:53
-0500 2023-12-11 03:22:52 INFO [3235595864 302ms] router: found process path: bound-socket
-0500 2023-12-11 03:22:52 DEBUG [3235595864 302ms] router: match[10] rule_set=cncidr => direct
-0500 2023-12-11 03:22:52 INFO [3235595864 302ms] outbound/direct[direct]: outbound connection to 114.114.114.114:53
```

### address bound socket: follow default routing rules
```sh
./bound-socket address_bound & sleep 1 && ss -tupan | grep bound
```

Output:
```text
[1] 18455
tcp   ESTAB  0      0               192.168.1.236:46799      114.114.114.114:53    users:(("bound-socket",pid=18455,fd=3))
```

The traffic enters the tun interface according to the default routing rules.  
Output from tun program:

```text
-0500 2023-12-11 03:24:11 INFO [3728153573 0ms] inbound/tun[tun-in]: inbound connection from 192.168.1.236:46799
-0500 2023-12-11 03:24:11 INFO [3728153573 0ms] inbound/tun[tun-in]: inbound connection to 114.114.114.114:53
-0500 2023-12-11 03:24:11 INFO [3728153573 301ms] router: found process path: bound-socket
-0500 2023-12-11 03:24:11 DEBUG [3728153573 302ms] router: match[10] rule_set=cncidr => direct
-0500 2023-12-11 03:24:11 INFO [3728153573 302ms] outbound/direct[direct]: outbound connection to 114.114.114.114:53
```

### interface bound socket: **interface constrained routing**
```sh
./bound-socket interface_bound & sleep 1 && ss -tupan | grep bound
```

Output:
```text
[1] 18555
tcp   ESTAB     0      0         192.168.1.236%ens33:56392      114.114.114.114:53    users:(("bound-socket",pid=18555,fd=3))
```

Output from tun program:   
**Nothing!** Because the traffic bypasses the tun interface.  