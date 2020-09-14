from rsa import crack_rsa
from prime import SieveOfEratosthenes
from math import *
import multiprocessing

'''
Threading wrapper to crack_rsa
'''
def crack_rsa_thread(i, return_dict, *args):
  return_dict[i] = crack_rsa(*args)

'''
main
'''
if __name__ == "__main__":
  e = 471131
  n = 19395215754271188593
  public_key = [e, n]
  groups = [
    1213428545899287006,
    128066609881241950,
    1524544144808352117,
    5739320063815598366,
    5318541268334496831,
    10099198048166427552,
    1294578297747087516,
    5219535363202696744,
    8873085490801871488,
    9767440483961474498,
    6090604250892558071,
    7230451814471929172,
    16327068223369301731,
    3332316276070559024,
  ]

  # First get all the prime numbers from 1 to sqrt(n)
  # where n is the biggest value in the groups.
  # This is what takes the most amount of time
  biggest = max(groups)
  limit = int(sqrt(biggest))
  print("Generating a list of all the primes from 1 to {}. This might take a while...".format(limit))
  primes = SieveOfEratosthenes(limit)
  print("Done, found {} prime numbers.".format(len(primes)))

  procs = len(groups) # Number of processes to create
  manager = multiprocessing.Manager()
  return_dict = manager.dict()

  # Create a list of jobs and then iterate through
  # the number of processes appending each process to
  # the job list 
  jobs = []
  for i in range(0, procs):
    crypted = groups[i]
    process = multiprocessing.Process(target=crack_rsa_thread, args=(i, return_dict, crypted, public_key, primes))
    jobs.append(process)

  # Start the processes   
  for j in jobs:
    j.start()

  # Ensure all of the processes have finished
  for j in jobs:
    j.join()

  print("List processing complete. Returned values: {}".format(return_dict.values()))