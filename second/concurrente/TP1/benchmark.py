import os
import re
import sys
import subprocess

PATH = "./bin/main"
OUTPUT = "./output.csv"
REGEX = re.compile(r"Processing image took ((?:\d+).(?:\d+)) ms.")

if __name__ == "__main__":
    if not os.path.isfile(PATH):
        raise Exception("{} does not exist".format(PATH))
    
    if not os.path.isfile(OUTPUT):
        os.system("rm -r {}".format(OUTPUT))

    images = [
        "./image.ppm",
        "./test.ppm",
    ]

    threads_max = 12

    kernels = [
        "identity",
        "sharpen",
        "edge",
        "blur",
        "gauss",
        "unsharp"
    ]

    full_percent = len(kernels) * len(images) * threads_max;
    progress = 0

    os.system("echo '' > {}".format(OUTPUT))

    for image in images:
        for kernel in kernels:
            for thread_count in range(1, threads_max + 1):
                progress += 1
                try:    
                    output = subprocess.check_output(["./bin/main", str(thread_count), image, kernel, "output.ppm"])
                    output = str(output)
                    ms = REGEX.findall(output)[0]
                    text = "{};{};{};{}".format(image, kernel, thread_count, ms)
                    os.system("echo '{}' >> {}".format(text, OUTPUT))
                    print("({}%) -> {}".format((100 * progress) / full_percent, text))
                except subprocess.CalledProcessError as e:
                    print(e.output)
        
        

    
