import { readFile } from 'fs/promises'

export async function content(filename: string) {
  const buffer = await readFile(filename)
  return buffer.toString()
}
