/**
 * A copy of python's ord function
 */
export function ord(input: string): number {
  return input.charCodeAt(0)
}

/**
 * A copy of python's max function
 */
export function max(a: number, b: number) {
  return a > b ? a : b
}
