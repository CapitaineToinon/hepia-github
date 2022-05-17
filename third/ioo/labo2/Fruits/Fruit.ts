import { FruitType } from "./FruitType";

export abstract class Fruit {
	#type: FruitType;
	#nom: string;
	#pepin: boolean;

	constructor({
		type,
		nom,
		pepin = false,
	}: {
		type: FruitType;
		nom: string;
		pepin: boolean;
	}) {
		this.#type = type;
		this.#nom = nom;
		this.#pepin = pepin;
	}

	get type(): FruitType {
		return this.#type;
	}

	set type(value: FruitType) {
		this.#type = value;
	}

	get nom(): string {
		return this.#nom;
	}

	set nom(value: string) {
		this.#nom = value;
	}

	get pepin(): boolean {
		return this.#pepin;
	}

	set pepin(value: boolean) {
		this.#pepin = value;
	}

	toString(): string {
		return `nom: ${this.#nom} - pepin: ${this.#pepin}`;
	}

	getCalories(): number {
		return this.type.getCalories(this);
	}
}
