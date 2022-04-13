import { Algorithms } from './algorithms'
import { IScanner, ScannerOptions } from './scanner'
import { RabinKarp } from './rabinkarp'
import { BoyerMoore } from './boyermoore'
import { KnuthMorrisPratt } from './knuthmorrispratt'

export function createScanner(
  algorithm: Algorithms,
  options: ScannerOptions = { verbose: false, zeroIndexed: false }
): IScanner {
  switch (algorithm) {
    case Algorithms.RabinKarp:
      return new RabinKarp(options)
    case Algorithms.KnuthMorrisPratt:
      return new KnuthMorrisPratt(options)
    case Algorithms.BoyerMoore:
      return new BoyerMoore(options)
    default:
      throw new Error('algorithm not implemented')
  }
}
