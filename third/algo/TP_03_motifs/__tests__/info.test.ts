import { Automates, KnuthMorrisPratt, Scanner } from '../src/scanners/index'

type Results = {
  scanner: Scanner
  patterns: {
    [pattern: string]: unknown
  }
}[]

const expected: Results = [
  {
    scanner: new Automates(),
    patterns: {
      '1212': [
        { '1': 1, '2': 0 },
        { '1': 1, '2': 2 },
        { '1': 3, '2': 0 },
        { '1': 1, '2': 4 },
        { '1': 3, '2': 0 },
      ],
      '1111abt1111ab': [
        { '1': 1, a: 0, b: 0, t: 0 },
        { '1': 2, a: 0, b: 0, t: 0 },
        { '1': 3, a: 0, b: 0, t: 0 },
        { '1': 4, a: 0, b: 0, t: 0 },
        { '1': 4, a: 5, b: 0, t: 0 },
        { '1': 1, a: 0, b: 6, t: 0 },
        { '1': 1, a: 0, b: 0, t: 7 },
        { '1': 8, a: 0, b: 0, t: 0 },
        { '1': 9, a: 0, b: 0, t: 0 },
        { '1': 10, a: 0, b: 0, t: 0 },
        { '1': 11, a: 0, b: 0, t: 0 },
        { '1': 4, a: 12, b: 0, t: 0 },
        { '1': 1, a: 0, b: 13, t: 0 },
        { '1': 1, a: 0, b: 0, t: 7 },
      ],
      bonsbonsbons: [
        { b: 1, o: 0, n: 0, s: 0 },
        { b: 1, o: 2, n: 0, s: 0 },
        { b: 1, o: 0, n: 3, s: 0 },
        { b: 1, o: 0, n: 0, s: 4 },
        { b: 5, o: 0, n: 0, s: 0 },
        { b: 1, o: 6, n: 0, s: 0 },
        { b: 1, o: 0, n: 7, s: 0 },
        { b: 1, o: 0, n: 0, s: 8 },
        { b: 9, o: 0, n: 0, s: 0 },
        { b: 1, o: 10, n: 0, s: 0 },
        { b: 1, o: 0, n: 11, s: 0 },
        { b: 1, o: 0, n: 0, s: 12 },
        { b: 9, o: 0, n: 0, s: 0 },
      ],
      wwww: [{ w: 1 }, { w: 2 }, { w: 3 }, { w: 4 }, { w: 4 }],
    },
  },
  {
    scanner: new KnuthMorrisPratt(),
    patterns: {
      '1212': [0, 0, 1, 2],
      '1111abt1111ab': [0, 1, 2, 3, 0, 0, 0, 1, 2, 3, 4, 5, 6],
      bonsbonsbons: [0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8],
      wwww: [0, 1, 2, 3],
    },
  },
]

expected.map(({ scanner, patterns }) => {
  for (const key in patterns) {
    test(`printInfo ${scanner.name} ${key}`, async () => {
      console.table = jest.fn()
      scanner.setPattern(key).printInfo()
      expect(console.table).toHaveBeenCalledWith(patterns[key])
    })
  }
})
