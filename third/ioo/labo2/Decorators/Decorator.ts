import { Fruit } from "../Fruits/Fruit";

export class Decorator extends Fruit {
	#fruit: Fruit;

	constructor({ fruit }: { fruit: Fruit }) {
		super(fruit);
		this.#fruit = fruit;
	}

	protected get fruit() {
		return this.#fruit;
	}
}
