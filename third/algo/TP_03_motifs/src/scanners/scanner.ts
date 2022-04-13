export interface IScanner {
  setPattern(pattern: string): this
  setSource(source: string): this
  getResult(): number[]
  scan(): this
  printResult(): this
  printInfo(): this
}

export interface ScannerOptions {
  verbose?: boolean
  zeroIndexed?: boolean
}

export abstract class Scanner implements IScanner {
  name: string = 'Unknown Scanner'
  pattern: string
  source?: string
  result: number[]
  verbose: boolean
  zeroIndexed: boolean

  constructor({ verbose, zeroIndexed }: ScannerOptions = { verbose: false, zeroIndexed: false }) {
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

  log(...args: any[]): void {
    if (!this.verbose) return
    console.log(...args)
  }

  printResult(): this {
    console.log({
      algorithm: this.name,
      pattern: this.pattern,
      occurances: this.result.length,
      positions: this.getResult(),
    })

    return this
  }

  abstract scan(): this
  abstract printInfo(): this
}
