import { Scanner } from './scanner'

export class KnuthMorrisPratt extends Scanner {
  name: string = 'Knuth-Morris-Pratt'

  #buildTable(): number[] {
    if (!this.source) {
      throw new Error('source is undefined')
    }

    const table: number[] = [0]

    let prefix = 0
    let suffix = 1

    while (suffix < this.pattern.length) {
      if (this.pattern[prefix] === this.pattern[suffix]) {
        table[suffix] = prefix + 1
        suffix += 1
        prefix += 1
      } else if (prefix === 0) {
        table[suffix] = 0
        suffix += 1
      } else {
        prefix = table[prefix - 1]
      }
    }

    return table
  }

  scan(): this {
    if (!this.source) {
      throw new Error('source is undefined')
    }

    this.log({ pattern: this.pattern, source: this.source })

    let sourceIdx = 0
    let patternIdx = 0

    const table = this.#buildTable()
    const positions: number[] = []

    while (sourceIdx < this.source.length) {
      if (this.source[sourceIdx] === this.pattern[patternIdx]) {
        if (patternIdx === this.pattern.length - 1) {
          const position = sourceIdx - this.pattern.length + 1
          this.log(`Found pattern '${this.pattern}' at position ${position}`)
          positions.push(position)
        }

        patternIdx += 1
        sourceIdx += 1
      } else if (patternIdx > 0) {
        patternIdx = table[patternIdx - 1]
      } else {
        sourceIdx += 1
      }
    }

    this.result = positions
    return this
  }

  printInfo(): this {
    console.dir(this.#buildTable())
    return this
  }
}