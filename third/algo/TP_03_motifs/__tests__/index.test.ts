import {
  Automates,
  RabinKarp,
  KnuthMorrisPratt,
  BoyerMoore,
} from '../src/scanners/index'
import { join } from 'path'
import { readFileSync } from 'fs'

const MOCK_SOURCE = readFileSync(join(__dirname, 'mock.txt')).toString()

;[RabinKarp, Automates, KnuthMorrisPratt, BoyerMoore].map((Scanner) => {
  const scanner = new Scanner()

  const patterns = {
    '1212': [2, 4, 6, 12, 98, 115, 199, 206, 208],
    wwww: [17, 18, 19, 29, 163, 212],
    bonsbonsbons: [34, 38, 42, 55, 182],
    '1111abt1111ab': [68, 81, 102, 150],
  }

  for (const key in patterns) {
    test(`${scanner.name} ${key}`, async () => {
      const actual = scanner
        .setPattern(key)
        .setSource(MOCK_SOURCE)
        .scan()
        .getResult()
      expect(actual).toStrictEqual(patterns[key])
    })
  }
})
