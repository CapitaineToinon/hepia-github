import { Fruit } from "./Fruit";

export abstract class FruitType extends Object {
	#nom: string;

	constructor({ nom }: { nom: string }) {
		super();
		this.nom = nom;
	}

	get nom(): string {
		return this.#nom;
	}

	set nom(value: string) {
		this.#nom = value;
	}

	abstract getCalories(fruit: Fruit): number;
}
