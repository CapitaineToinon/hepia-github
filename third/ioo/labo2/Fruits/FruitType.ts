import { Fruit } from "./Fruit";

export abstract class FruitType {
	#nom: string;

	constructor({ nom }: { nom: string }) {
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
