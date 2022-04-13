import type { IScanner } from './scanners/scanner'
import { RabinKarp, KnuthMorrisPratt, Scanners } from './scanners/index'

export default class Factory {
  static create(type: Scanners, pattern: string, verbose: boolean): IScanner {
    switch (type) {
      case Scanners.RabinKarp:
        return new RabinKarp(pattern, verbose)
      case Scanners.KnuthMorrisPratt:
        return new KnuthMorrisPratt(pattern, verbose)
      default:
        throw new Error('algorithm not implemented')
    }
  }
}
