from dotenv import load_dotenv
load_dotenv()

import googleapiclient.discovery
import argparse
import os
import time

compute = googleapiclient.discovery.build('compute', 'v1')

project = 'cloud-du-cul'
zone = 'europe-west2-a'
name = 'pyvm'
bucket = 'pybucket'


if __name__ == '__main__':
    # Get the latest Debian Jessie image.
    image_response = compute.images().getFromFamily(project='ubuntu-os-cloud', family='ubuntu-1804-lts').execute()
    source_disk_image = image_response['selfLink']

    # Configure the machine
    machine_type = "zones/{zone}/machineTypes/n1-standard-1".format(zone=zone)
    # startup_script = open(
    #     os.path.join(
    #         os.path.dirname(__file__), 'startup-script.sh'), 'r').read()
    # image_url = "http://storage.googleapis.com/gce-demo-input/photo.jpg"
    # image_caption = "Ready for dessert?"

    config = {
        'name': name,
        'machineType': machine_type,

        # Specify the boot disk and the image to use as a source.
        'disks': [
            {
                'boot': True,
                'autoDelete': True,
                'initializeParams': {
                    'sourceImage': source_disk_image,
                }
            }
        ],

        # Specify a network interface with NAT to access the public
        # internet.
        'networkInterfaces': [{
            'network': 'global/networks/default',
            'accessConfigs': [
                {'type': 'ONE_TO_ONE_NAT', 'name': 'External NAT'}
            ]
        }],

        # Allow the instance to access cloud storage and logging.
        'serviceAccounts': [{
            'email': 'default',
            'scopes': [
                'https://www.googleapis.com/auth/devstorage.read_write',
                'https://www.googleapis.com/auth/logging.write'
            ]
        }],

        # Metadata is readable from the instance and allows you to
        # pass configuration from deployment scripts to instances.
        'metadata': {
            # 'items': [{
            #     # Startup script is automatically executed by the
            #     # instance upon startup.
            #     'key': 'startup-script',
            #     'value': startup_script
            # }, {
            #     'key': 'url',
            #     'value': image_url
            # }, {
            #     'key': 'text',
            #     'value': image_caption
            # }, {
            #     'key': 'bucket',
            #     'value': bucket
            # }]
        }
    }

    compute.instances().insert(
        project=project,
        zone=zone,
        body=config).execute()