import { Scanner } from './scanner'

type State = number

type CharacterStateMap = {
  [key: string]: State
}

export class Automates extends Scanner {
  name: string = 'Automates'
  #table: CharacterStateMap[]

  scan(): this {
    this.checkBeforeScan()

    const positions: number[] = []

    let state = 0
    for (let i = 0; i < this.source.length; i++) {
      // fallback to state 0 if we go out of bounds
      // aka if the character at i isn't part of the pattern at all
      state = this.#table[state][this.source[i]] ?? 0
      if (state === this.pattern.length) {
        const position = i - this.pattern.length + 1
        this.logPattern(position)
        positions.push(position)
      }
    }

    this.result = positions
    return this
  }

  #getNextState(pattern: string, state: number, key: string) {
    const m = pattern.length
    // If the character c is same as next character
    // in pattern,then simply increment state
    if (state < m && key === pattern[state]) {
      return state + 1
    }

    // ns stores the result which is next state
    let ns: number
    let i: number

    // ns finally contains the longest prefix
    // which is also suffix in "pat[0..state-1]c"

    // Start from the largest possible value
    // and stop when you find a prefix which
    // is also suffix
    for (ns = state; ns > 0; ns--) {
      if (key === pattern[ns - 1]) {
        for (i = 0; i < ns - 1; i++) {
          if (pattern[i] !== pattern[state - ns + 1 + i]) {
            break
          }
        }

        if (i === ns - 1) {
          return ns
        }
      }
    }

    return 0
  }

  override setPattern(pattern: string): this {
    this.#table = []
    const uniqChars = [...new Set(pattern.split(''))]

    // very important to loop on state <= pattern.length
    // to create a final step that allows looping back
    // on potentially overlapping patterns!
    for (let state = 0; state <= pattern.length; ++state) {
      this.#table[state] = uniqChars.reduce(
        (previous, key) => ({
          ...previous,
          [key]: this.#getNextState(pattern, state, key),
        }),
        {}
      )
    }

    super.setPattern(pattern)
    return this
  }

  printInfo(): this {
    console.table(this.#table)
    return this
  }
}
