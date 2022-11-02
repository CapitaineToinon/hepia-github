from dotenv import load_dotenv
load_dotenv()

import requests
import exoscale

key = "pute"

if __name__ == '__main__':
    exo = exoscale.Exoscale()

    # exoscale sucks I have to do this I think
    bucket = None
    security_group_web = None
    elastic_ip = None
    domain = None
    instance = None

    try:
        bucket = exo.storage.get_bucket("{0}-bucket".format(key))
    except exoscale.api.ResourceNotFoundError:
        bucket = exo.storage.create_bucket("{0}-bucket".format(key), zone="ch-gva-2")

    file_index = bucket.put_file("index.html.tpl")
    file_index.set_acl('public-read')

    zone_gva2 = exo.compute.get_zone("ch-gva-2")

    try:
        security_group_web = exo.compute.get_security_group("{0}-security-group".format(key))
    except exoscale.api.ResourceNotFoundError:
        security_group_web = exo.compute.create_security_group("{0}-security-group".format(key))
        rules = [
            exoscale.api.compute.SecurityGroupRule.ingress(
                description="HTTP",
                network_cidr="0.0.0.0/0",
                port="80",
                protocol="tcp",
            ),
            exoscale.api.compute.SecurityGroupRule.ingress(
                description="HTTPS",
                network_cidr="0.0.0.0/0",
                port="443",
                protocol="tcp",
            ),
        ]
        [security_group_web.add_rule(rule) for rule in rules]
    
    elastic_ip = exo.compute.create_elastic_ip(zone_gva2)

    try:
        instance = exo.compute.get_instance(
            id="web1",
            zone=zone_gva2,
            ip_address=elastic_ip.address,
        )
    except exoscale.api.ResourceNotFoundError:
        instance = exo.compute.create_instance(
            name="web1",
            zone=zone_gva2,
            type=exo.compute.get_instance_type("medium"),
            template=list(
                exo.compute.list_instance_templates(
                    zone_gva2,
                    "Linux Ubuntu 18.04 LTS 64-bit"))[0],
            volume_size=50,
            security_groups=[security_group_web],
            user_data="""#cloud-config
        package_upgrade: true
        packages:
        - nginx
        write_files:
        - path: /etc/netplan/51-eip.yaml
        content: |
            network:
            version: 2
            renderer: networkd
            ethernets:
                lo:
                match:
                    name: lo
                addresses:
                    - {eip}/32
        runcmd:
        - curl {template_url} | sed -re "s/__ME__/$(hostname)/" > /var/www/html/index.html
        """.format(
            eip=elastic_ip.address,
            template_url=file_index.url),
        )

    print(instance.id)
    print(instance.name)
    print(instance.type.name)
    print(instance.template.name)
    print(instance.ipv4_address)
    print(instance.zone.name)
    
    instance.attach_elastic_ip(elastic_ip)
    for eip in instance.elastic_ips:
        print(eip.address)

    try:
        domain = exo.dns.get_domain("{0}.com".format(key))
    except exoscale.api.ResourceNotFoundError:
        domain = exo.dns.create_domain("{0}.com".format(key))

    record_web1 = domain.add_record(instance.name + "." + domain.name, "A", elastic_ip.address)
    for rec in (r for r in domain.records if r.type not in {"SOA", "NS"}):
        print(rec.name, rec.type, rec.content)

    res = requests.get("http://{subdomain}.{domain}.com".format(domain=domain.name, subdomain=instance.name))
    print(res.text)
