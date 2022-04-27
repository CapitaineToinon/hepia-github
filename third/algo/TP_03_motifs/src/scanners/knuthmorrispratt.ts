import { Scanner } from './scanner'

export class KnuthMorrisPratt extends Scanner {
  name = 'Knuth-Morris-Pratt'
  #table: number[]

  #buildTable() {
    this.checkPattern()
    const table: number[] = [0]

    let prefix = 0
    let suffix = 1

    while (suffix < this.pattern.length) {
      if (this.pattern[prefix] === this.pattern[suffix]) {
        table[suffix] = prefix + 1
        suffix++
        prefix++
      } else if (prefix === 0) {
        table[suffix] = 0
        suffix++
      } else {
        prefix = table[prefix - 1]
      }
    }

    this.#table = table
  }

  scan(): this {
    this.checkSource()
    this.checkPattern()

    let sourceIdx = 0
    let patternIdx = 0

    const positions: number[] = []

    while (sourceIdx < this.source.length) {
      if (this.source[sourceIdx] === this.pattern[patternIdx]) {
        if (patternIdx === this.pattern.length - 1) {
          const position = sourceIdx - this.pattern.length + 1
          this.logPattern(position)
          positions.push(position)
        }

        patternIdx++
        sourceIdx++
      } else if (patternIdx > 0) {
        patternIdx = this.#table[patternIdx - 1]
      } else {
        sourceIdx++
      }
    }

    this.result = positions
    return this
  }

  override setPattern(pattern: string): this {
    super.setPattern(pattern)
    this.#buildTable()
    return this
  }

  printInfo(): this {
    console.table(this.#table)
    return this
  }
}
