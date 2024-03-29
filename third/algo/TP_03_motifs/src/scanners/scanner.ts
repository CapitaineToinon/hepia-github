export interface IScanner {
  setPattern(pattern: string): this
  setSource(source: string): this
  getResult(): number[]
  scan(): this
  printInfo(): this
}

export interface ScannerOptions {
  verbose?: boolean
  zeroIndexed?: boolean
}

export abstract class Scanner implements IScanner {
  name = 'Unknown Scanner'
  pattern: string
  source?: string
  result: number[]
  verbose: boolean
  zeroIndexed: boolean

  constructor(
    { verbose, zeroIndexed }: ScannerOptions = {
      verbose: false,
      zeroIndexed: false,
    }
  ) {
    this.verbose = verbose
    this.zeroIndexed = zeroIndexed
  }

  setPattern(pattern: string) {
    this.pattern = pattern
    return this
  }

  setSource(source: string) {
    this.source = source
    return this
  }

  getResult(): number[] {
    if (!this.zeroIndexed) {
      return this.result.map((i) => i + 1)
    }

    return this.result
  }

  log(...args: unknown[]): void {
    if (!this.verbose) return
    console.log(...args)
  }

  logPattern(position: number) {
    this.log(`Found pattern '${this.pattern}' at position ${position}`)
  }

  checkSource() {
    if (!this.source) {
      throw new Error('source is undefined')
    }
  }

  checkPattern() {
    if (!this.pattern) {
      throw new Error('pattern is undefined')
    }
  }

  abstract scan(): this
  abstract printInfo(): this
}
