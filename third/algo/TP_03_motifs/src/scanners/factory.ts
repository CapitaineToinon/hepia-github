import { Algorithms } from './algorithms'
import { IScanner } from './scanner'
import { RabinKarp } from './rabinkarp'
import { KnuthMorrisPratt } from './knuthmorrispratt'

export function createScanner(algorithm: Algorithms, pattern: string, verbose: boolean): IScanner {
  switch (algorithm) {
    case Algorithms.RabinKarp:
      return new RabinKarp(pattern, verbose)
    case Algorithms.KnuthMorrisPratt:
      return new KnuthMorrisPratt(pattern, verbose)
    default:
      throw new Error('algorithm not implemented')
  }
}
