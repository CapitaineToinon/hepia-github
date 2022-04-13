import { Scanner } from './scanner'

type CharacterStateMap = {
  [key: string]: number
}

export class Automates extends Scanner {
  name = 'Automates'
  #table: CharacterStateMap[]

  scan(): this {
    this.checkBeforeScan()

    const positions: number[] = []

    // naviguate the state machine...
    let state = 0
    for (let i = 0; i < this.source.length; i++) {
      // fallback to state 0 if the character at this.source[i] isn't
      // part of the pattern and thus isn't part of the state machine table.
      // defaults back to state 0 if character not found.
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
    //
    // we're also only creating entries for unique chars
    // in the pattern. foreign characters needs to be handled
    // by the search function instead.
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
