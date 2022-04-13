export interface IScanner {
  setSource(source: string): this
  scan(): this
  printResult(): this
  printInfo(): this
}

export abstract class Scanner implements IScanner {
  name: string
  pattern: string
  source?: string
  verbose: boolean
  result: number[]

  constructor(pattern: string, verbose: boolean = false) {
    this.pattern = pattern
    this.verbose = verbose
  }

  setSource(source: string) {
    this.source = source
    return this
  }

  log(...args: any[]): void {
    if (!this.verbose) return
    console.log(...args)
  }

  printResult(): this {
    this.log({
      algorithm: this.name,
      pattern: this.pattern,
      occurances: this.result.length,
      position: this.result.map((i) => i + 1), // not using 0 indexed in output
    })

    return this
  }

  abstract scan(): this
  abstract printInfo(): this
}
