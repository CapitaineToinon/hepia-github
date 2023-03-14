export function format_node(node) {
  return `Host ${node.name}
    User root
    Hostname ${node.console_host}
    Port ${node.console + 1}
    StrictHostKeychecking no
    UserKnownHostsFile /dev/null
    IdentityFile ~/.ssh/gns3.rsa`;
}
