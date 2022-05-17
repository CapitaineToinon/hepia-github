class MyArray<T> extends Array<T> {
	override map<U>(callbackfn: (value: T, index: number, array: T[]) => U): U[] {
		console.log("calling custom map");
		return this.reduce((previousValue, currentValue, currentIndex) => {
			return [...previousValue, callbackfn(currentValue, currentIndex, this)];
		}, [] as U[]);
	}

	override filter(
		predicate: (value: T, index: number, array: T[]) => boolean
	): T[] {
		console.log("calling custom filter");
		return this.reduce((previousValue, currentValue, currentIndex) => {
			return predicate(currentValue, currentIndex, this)
				? [...previousValue, currentValue]
				: previousValue;
		}, [] as T[]);
	}

	override reverse(): T[] {
		console.log("calling custom reverse");
		return this.reduce((previousValue, currentValue, _) => {
			return [currentValue, ...previousValue];
		}, [] as T[]);
	}

	override every(
		predicate: (value: T, index: number, array: T[]) => boolean
	): boolean {
		console.log("calling custom every");
		const count = this.reduce((previousValue, currentValue, currentIndex) => {
			return predicate(currentValue, currentIndex, this)
				? previousValue + 1
				: previousValue;
		}, 0);

		return count === this.length;
	}

	unique(): T[] {
		console.log("calling custom unique");
		return this.reduce((previousValue, currentValue, _) => {
			return !previousValue.includes(currentValue)
				? [...previousValue, currentValue]
				: previousValue;
		}, [] as T[]);
	}
}

const array = new MyArray<number>();
array.push(1, 2, 3, 4, 5, 2);

console.log(array)
console.log(array.map((v) => v * 2));
console.log(array.filter((v, i, a) => v % 2 === 0));
console.log(array.reverse());
console.log(array.every((v) => v > 0));
console.log(array.every((v) => v > 2));
console.log(array.unique());
