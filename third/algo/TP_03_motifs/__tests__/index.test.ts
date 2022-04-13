import { RabinKarp, KnuthMorrisPratt, BoyerMoore } from '../src/scanners/index'
import { join } from 'path'
import { readFileSync } from 'fs'

const MOCK_SOURCE = readFileSync(join(__dirname, 'mock.txt')).toString()
const classes = [RabinKarp, KnuthMorrisPratt, BoyerMoore]

classes.map((Scanner) => {
  const scanner = new Scanner()

  const getActual = (pattern: string) => {
    return scanner.setPattern(pattern).setSource(MOCK_SOURCE).scan().getResult()
  }

  test(`${scanner.name} 1212`, async () => {
    const actual = getActual('1212')
    const expected = [2, 4, 6, 12, 98, 115, 199, 206, 208]
    expect(actual).toStrictEqual(expected)
  })

  test(`${scanner.name} wwww`, async () => {
    const actual = getActual('wwww')
    const expected = [17, 18, 19, 29, 163, 212]
    expect(actual).toStrictEqual(expected)
  })

  test(`${scanner.name} bonsbonsbons`, async () => {
    const actual = getActual('bonsbonsbons')
    const expected = [34, 38, 42, 55, 182]
    expect(actual).toStrictEqual(expected)
  })

  test(`${scanner.name} 1111abt1111ab`, async () => {
    const actual = getActual('1111abt1111ab')
    const expected = [68, 81, 102, 150]
    expect(actual).toStrictEqual(expected)
  })
})
