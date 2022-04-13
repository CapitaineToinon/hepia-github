import { Scanner } from './scanner'
import { ord } from '../python/index'

export class RabinKarp extends Scanner {
  name: string = 'Rabin-Karp'
  #prime: number = 337 // https://xkcd.com/221/
  #setSize: number = 256 // amount of different possible characters in ascii

  scan() {
    this.checkBeforeScan()

    // to match names in literature
    let d = this.#setSize
    let p = this.#prime

    let pHash: number = 0 // pattern hash
    let sHash: number = 0 // source hash
    let h: number = 1

    // result array!
    const positions: number[] = []

    // The value of h would be "pow(d, M - 1) % q"
    for (let i = 0; i < this.pattern.length - 1; i++) {
      h = (h * d) % p
    }

    // Calculate the hash value of pattern and first
    // window of text
    for (let i = 0; i < this.pattern.length; i++) {
      pHash = (d * pHash + ord(this.pattern[i])) % p
      sHash = (d * sHash + ord(this.source[i])) % p
    }

    // slide pattern over the source
    for (let i = 0, j = 0; i <= this.source.length - this.pattern.length; i++) {
      if (pHash == sHash) {
        // we need j after the for loop
        for (j = 0; j < this.pattern.length; j++) {
          if (this.source[i + j] != this.pattern[j]) {
            break
          }
        }

        if (j == this.pattern.length) {
          this.logPattern(i)
          positions.push(i)
        }
      }

      // Calculate hash value for next window of text: Remove
      // leading digit, add trailing digit
      if (i < this.source.length - this.pattern.length) {
        sHash =
          (d * (sHash - ord(this.source[i]) * h) +
            ord(this.source[i + this.pattern.length])) %
          p

        // ensure sHash is positive
        if (sHash < 0) {
          sHash = sHash + p
        }
      }
    }

    this.result = positions
    return this
  }

  printInfo(): this {
    console.log('info')
    return this
  }
}
