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

# Labo 3

The first step is as usual to configure ssh to enable the ssh connections. Once this is done, ansible has been install with python using a python venv. You can install the requirements listed in requirements.txt with the following command:

```
pip install -r requirements.txt
```

Then configure all the IPs for H1, H2, R1 and R2.

```
ssh H1 ip addr add 1.0.0.3/24 dev eth0
ssh H1 ip link set up dev eth0

ssh R1 ip addr add 1.0.0.1/24 dev eth1
ssh R1 ip link set up dev eth1
ssh R1 ip addr add 2.0.0.1/24 dev eth0
ssh R1 ip link set up dev eth0

ssh R2 ip addr add 2.0.0.2/24 dev eth0
ssh R2 ip link set up dev eth0
ssh R2 ip addr add 3.0.0.2/24 dev eth1
ssh R2 ip link set up dev eth1

ssh H2 ip addr add 3.0.0.3/24 dev eth0
ssh H2 ip link set up dev eth0
```

# ping

We can then test the base setup by running ansible:

```
ansible -m ping -i "H1,H2,R1,R2" all
```

And see that everything is working as expected.

```
R1 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
H2 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
R2 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
H1 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": false,
    "ping": "pong"
}
```

The command ansible -m ping -i "H1,H2,R1,R2" all uses Ansible to ping the specified hosts. The -m ping flag specifies that the ping module should be used. The -i "H1,H2,R1,R2" flag specifies the inventory of hosts to be used, in this case H1, H2, R1, and R2. The all at the end specifies that the command should be run on all hosts in the inventory. This command will attempt to ping all four hosts and return the results.

# uptime

To display the uptime of all machines without writing an inventory file, you can use the following ad-hoc command:

```
ansible all -i "H1,H2,R1,R2," -m command -a "uptime"
```

```
R1 | CHANGED | rc=0 >>
 09:42:29 up 31 min,  2 users,  load average: 0.00, 0.00, 0.00
H2 | CHANGED | rc=0 >>
 09:42:29 up 31 min,  2 users,  load average: 0.01, 0.02, 0.00
R2 | CHANGED | rc=0 >>
 09:42:29 up 31 min,  2 users,  load average: 0.00, 0.00, 0.00
H1 | CHANGED | rc=0 >>
 09:42:29 up 31 min,  2 users,  load average: 0.00, 0.00, 0.00
```

This command uses the command module to run the uptime command on all hosts specified in the inventory. The -i "H1,H2,R1,R2," flag specifies the inventory of hosts to be used, in this case H1, H2, R1, and R2. The trailing comma is necessary to indicate that this is a list of hosts rather than a path to an inventory file.

# hello.txt

To create the file /tmp/hello.txt on all machines using an ad-hoc command, you can use the following command:

```
ansible all -i "H1,H2,R1,R2," -m file -a "path=/tmp/hello.txt state=touch"
```

```
R1 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": true,
    "dest": "/tmp/hello.txt",
    "gid": 0,
    "group": "root",
    "mode": "0644",
    "owner": "root",
    "size": 0,
    "state": "file",
    "uid": 0
}
H2 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": true,
    "dest": "/tmp/hello.txt",
    "gid": 0,
    "group": "root",
    "mode": "0644",
    "owner": "root",
    "size": 0,
    "state": "file",
    "uid": 0
}
R2 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": true,
    "dest": "/tmp/hello.txt",
    "gid": 0,
    "group": "root",
    "mode": "0644",
    "owner": "root",
    "size": 0,
    "state": "file",
    "uid": 0
}
H1 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python3"
    },
    "changed": true,
    "dest": "/tmp/hello.txt",
    "gid": 0,
    "group": "root",
    "mode": "0644",
    "owner": "root",
    "size": 0,
    "state": "file",
    "uid": 0
}
```

We can see that the files has been successfully created on all machines.

![hello.txt](hello.png)

# shell vs raw

The shell and raw modules in Ansible are both used to execute commands on remote hosts. The main difference between the two is that the shell module executes commands in a shell on the remote host via Python, while the raw module executes commands directly via SSH without using a shell or Python.

Here is an example of using the shell module to run a command on a remote host:

```
ansible all -i "H1,H2,R1,R2," -m shell -a "ls -l /tmp"
```

This command uses the shell module to run the ls -l /tmp command on all hosts specified in the inventory. The -i "H1,H2,R1,R2," flag specifies the inventory of hosts to be used, in this case H1, H2, R1, and R2. The trailing comma is necessary to indicate that this is a list of hosts rather than a path to an inventory file. The -a "ls -l /tmp" flag specifies the arguments to be passed to the shell module, in this case the command to be run.

Here is an example of using the raw module to run a command on a remote host:

```
ansible all -i "H1,H2,R1,R2," -m raw -a "ls -l /tmp"
```

This command uses the raw module to run the ls -l /tmp command on all hosts specified in the inventory. The -i "H1,H2,R1,R2," flag specifies the inventory of hosts to be used, in this case H1, H2, R1, and R2. The trailing comma is necessary to indicate that this is a list of hosts rather than a path to an inventory file. The -a "ls -l /tmp" flag specifies the arguments to be passed to the raw module, in this case the command to be run.

The main advantage of using the raw module over the shell module is that it can be used on systems that do not have a Python interpreter installed, since it does not require any additional modules or libraries. However, since it does not use a shell or Python, it does not support shell features such as variable expansion or command piping.

# Routing


