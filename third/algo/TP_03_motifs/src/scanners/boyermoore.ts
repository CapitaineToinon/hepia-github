import { Scanner } from './scanner'
import { max, ord } from '../python/index'

export class BoyerMoore extends Scanner {
  name = 'Boyer-Moore'
  #setSize = 256 // amount of different possible characters in ascii

  // The preprocessing function for Boyer Moore's
  // bad character heuristic
  #badCharHeuristic(str: string, size: number, badchar: number[]) {
    // Initialize all occurrences as -1
    for (let i = 0; i < this.#setSize; i++) {
      badchar[i] = -1
    }

    // Fill the actual value of last occurrence
    // of a character (indices of table are ascii and values are index of occurrence)
    for (let i = 0; i < size; i++) {
      badchar[str[i].charCodeAt(0)] = i
    }
  }

  scan(): this {
    this.checkBeforeScan()

    const badchar = new Array(this.#setSize)
    const positions: number[] = []

    /* Fill the bad character array by calling
       the preprocessing function badCharHeuristic()
       for given pattern */
    this.#badCharHeuristic(this.pattern, this.pattern.length, badchar)

    let s = 0 // s is shift of the pattern with
    // respect to text
    // there are n-m+1 potential alignments
    while (s <= this.source.length - this.pattern.length) {
      let j = this.pattern.length - 1

      /* Keep reducing index j of pattern while
         characters of pattern and text are
         matching at this shift s */
      while (j >= 0 && this.pattern[j] == this.source[s + j]) {
        j--
      }

      /* If the pattern is present at current
         shift, then index j will become -1 after
         the above loop */
      if (j < 0) {
        this.logPattern(s)
        positions.push(s)

        /* Shift the pattern so that the next
           character in text aligns with the last
           occurrence of it in pattern.
           The condition s+m < n is necessary for
           the case when pattern occurs at the end
           of text */
        // txt[s+m] is character after the pattern in text
        s +=
          s + this.pattern.length < this.source.length
            ? this.pattern.length -
              badchar[ord(this.source[s + this.pattern.length])]
            : 1
      } else
      /* Shift the pattern so that the bad character
         in text aligns with the last occurrence of
         it in pattern. The max function is used to
         make sure that we get a positive shift.
         We may get a negative shift if the last
         occurrence  of bad character in pattern
         is on the right side of the current
         character. */
        s += max(1, j - badchar[ord(this.source[s + j])])
    }

    this.result = positions
    return this
  }

  printInfo(): this {
    console.log('info')
    return this
  }
}
