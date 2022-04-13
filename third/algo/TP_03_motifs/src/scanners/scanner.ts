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
    let positions = this.result

    if (!this.zeroIndexed) {
      positions = positions.map((i) => i + 1)
    }

    return positions
  }

  log(...args: unknown[]): void {
    if (!this.verbose) return
    console.log(...args)
  }

  logPattern(position: number) {
    this.log(`Found pattern '${this.pattern}' at position ${position}`)
  }

  checkBeforeScan() {
    if (!this.source) {
      throw new Error('source is undefined')
    }

    if (!this.pattern) {
      throw new Error('pattern is undefined')
    }

    this.log({ pattern: this.pattern, source: this.source })
  }

  abstract scan(): this
  abstract printInfo(): this
}
