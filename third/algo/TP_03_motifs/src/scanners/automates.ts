import { Scanner } from './scanner'

type CharacterStateMap = {
  [key: string]: number
}

export class Automates extends Scanner {
  name = 'Automates'
  #table: CharacterStateMap[]

  #buildTable() {
    this.checkPattern()

    const uniqChars = [...new Set(this.pattern.split(''))]

    // very important to loop on state <= pattern.length
    // to create a final step that allows looping back
    // on potentially overlapping patterns!
    //
    // we're also only creating entries for unique chars
    // in the pattern. foreign characters needs to be handled
    // by the search function instead.
    this.#table = []

    for (let state = 0; state <= this.pattern.length; state++) {
      this.#table[state] = uniqChars.reduce(
        (previous, key) => ({
          ...previous,
          [key]: this.#getNextState(state, key),
        }),
        {}
      )
    }
  }

  scan(): this {
    this.checkSource()
    this.checkPattern()

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

  #getNextState(state: number, key: string) {
    // If current character is same as next character
    // in the pattern then just increment the state
    if (state < this.pattern.length && key === this.pattern[state]) {
      return state + 1
    }

    let nextState: number

    // next state finally contains the longest prefix
    // which is also suffix in "pat[0..state-1]c"

    // Start from the largest possible value
    // and stop when you find a prefix which
    // is also suffix
    for (nextState = state; nextState > 0; nextState--) {
      if (key === this.pattern[nextState - 1]) {
        let i: number // we need it after the loop
        for (i = 0; i < nextState - 1; i++) {
          if (this.pattern[i] !== this.pattern[state - nextState + 1 + i]) {
            break
          }
        }

        if (i === nextState - 1) {
          return nextState
        }
      }
    }

    return 0
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
