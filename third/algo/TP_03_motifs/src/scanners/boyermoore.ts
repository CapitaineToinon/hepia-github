import { Scanner } from './scanner'
import { max, ord } from '../python/index'

export class BoyerMoore extends Scanner {
  name = 'Boyer-Moore'
  #setSize = 65536 // max possible values
  #charTable: number[]
  #offsetTable: number[]

  scan(): this {
    this.checkPattern()
    this.checkSource()

    this.result = []

    for (let i = this.pattern.length - 1; i < this.source.length; ) {
      let j = this.pattern.length - 1

      for (; this.pattern[j] == this.source[i]; i--, j--) {
        if (j === 0) {
          this.logPattern(i)
          this.result.push(i)
          break
        }
      }

      const a = this.#offsetTable[this.pattern.length - 1 - j]
      const b = this.#charTable[ord(this.source[i])]
      i += max(a, b)
    }

    return this
  }

  #buildTables() {
    this.#buildCharTable()
    this.#buildOffsetTable()
  }

  #buildCharTable() {
    this.checkPattern()

    // fill the table
    this.#charTable = new Array(this.#setSize).fill(this.pattern.length)

    for (let i = 0; i < this.pattern.length - 1; i++) {
      this.#charTable[ord(this.pattern[i])] = this.pattern.length - 1 - i
    }
  }

  #buildOffsetTable() {
    this.checkPattern()

    this.#offsetTable = new Array(this.pattern.length)

    let last = this.pattern.length

    for (let i = this.pattern.length; i > 0; i--) {
      if (this.#isPrefix(i)) {
        last = i
      }

      this.#offsetTable[this.pattern.length - i] =
        last - 1 + this.pattern.length
    }

    for (let i = 0; i < this.pattern.length - 1; i++) {
      const length = this.#getSuffixLength(i)
      this.#offsetTable[length] = this.pattern.length - 1 - i + length
    }
  }

  #isPrefix(n: number) {
    for (let i = n, j = 0; i < this.pattern.length; i++, j++) {
      if (this.pattern[i] !== this.pattern[j]) {
        return false
      }
    }

    return true
  }

  #getSuffixLength(n: number) {
    let length = 0

    let i = n
    let j = this.pattern.length - 1

    while (i >= 0 && this.pattern[i] == this.pattern[j]) {
      length++
      i--
      j--
    }

    // for (
    //   let i = n, j = this.pattern.length - 1;
    //   i >= 0 && this.pattern[i] == this.pattern[j];
    //   i--, j--
    // ) {
    //   length++
    // }

    return length
  }

  override setPattern(pattern: string): this {
    super.setPattern(pattern)
    this.#buildTables()
    return this
  }

  printInfo(): this {
    console.table(this.#charTable)
    console.table(this.#offsetTable)
    return this
  }
}
