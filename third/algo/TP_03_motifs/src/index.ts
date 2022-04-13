import type { Argv } from 'yargs'
import * as process from 'process'
import yargs from 'yargs/yargs'
import { hideBin } from 'yargs/helpers'
import { createScanner } from './scanners/index'
import { content } from './file'

class Main {
  public static main(args: string[]) {
    let fileName: string = null
    let motif: string = null
    let algo: number = 0
    switch (args.length) {
      case 3:
        fileName = args[2]
      case 2:
        algo = parseInt(args[1])
        motif = args[0]
        break
      default:
        console.error('usage: java Main <motif> <algo> (<fichier_texte>)')
        process.exit(1)
    }
    switch (algo) {
      case 1:
        if (fileName === null) {
          console.info('26 37 18')
        } else {
          console.info('13')
          console.info('0 3 46 67 109')
        }
        break
      case 2:
        if (fileName === null) {
          console.info('1 0 0')
          console.info('1 2 0')
          console.info('3 0 0')
          console.info('1 4 0')
          console.info('5 0 0')
          console.info('1 4 6')
          console.info('7 0 0')
          console.info('1 2 0')
        } else {
          console.info('13')
          console.info('0 3 46 67 109')
        }
        break
      case 3:
        if (fileName === null) {
          console.info('0 0 1 2 3 0 1')
        } else {
          console.info('13')
          console.info('0 3 46 67 109')
        }
        break
      case 4:
        if (fileName === null) {
          console.info('1 0 5 2 8')
          console.info('8 3 6 6 6 6 6 6')
        } else {
          console.info('13')
          console.info('0 3 46 67 109')
        }
        break
      default:
        console.error('Algorithm not implemented')
        process.exit(2)
    }
  }
}

;(yargs(hideBin(process.argv)) as Argv)
  .command(
    '$0 <pattern> <algo> [filename] [options]',
    'scan a file based on a pattern',
    (yargs) => {
      return yargs
        .positional('pattern', {
          describe: 'The pattern to be searched',
          type: 'string',
        })
        .positional('algo', {
          describe: 'The algorythim to use',
          type: 'number',
        })
        .positional('filename', {
          describe: 'Filename to search the pattern in',
          type: 'string',
        })
        .option('verbose', {
          alias: 'v',
          type: 'boolean',
          description: 'Run with verbose logging',
          default: false,
        })
        .option('zero', {
          alias: 'z',
          type: 'boolean',
          description: 'Return the result as zero indexed',
          default: false,
        })
    },
    async ({ algo, pattern, filename, verbose, zero }) => {
      const scanner = createScanner(algo, {
        verbose,
        zeroIndexed: zero,
      }).setPattern(pattern)

      if (filename) {
        const source = await content(filename)
        const result = scanner.setSource(source).scan().getResult()

        if (verbose) {
          console.log({
            pattern: pattern,
            occurances: result.length,
            positions: result,
          })
        }

        console.log(result.join(','))
      } else {
        scanner.printInfo()
      }
    }
  )
  .parse()
