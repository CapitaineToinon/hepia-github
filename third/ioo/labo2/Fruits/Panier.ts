import { Fruit } from "./Fruit";
export class Panier extends Fruit {
	#childs: Fruit[];

	constructor({ childs }: { childs: Fruit[] }) {
		super({
			type: null,
			nom: "Panier de fruit",
			pepin: false,
		});

		this.#childs = childs;
	}

	override get pepin(): boolean {
		return this.#childs.some((fruit) => fruit.pepin);
	}

	override toString(): string {
		return `Panier (${this.#childs.map((fruit) => fruit.nom).join(" ")})`;
	}

	override getCalories(): number {
		return this.#childs.reduce((prev, fruit) => prev + fruit.getCalories(), 0);
	}
}
