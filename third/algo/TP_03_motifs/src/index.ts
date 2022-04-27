import type { Argv } from 'yargs'
import * as process from 'process'
import yargs from 'yargs/yargs'
import { hideBin } from 'yargs/helpers'
import { createScanner } from './scanners/index'
import { content } from './file'

const args: Argv = yargs(hideBin(process.argv))

args
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
