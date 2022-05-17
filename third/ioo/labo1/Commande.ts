import { Fruit } from "./Fruits/Fruit";

export class Commande {
	static #instance: Commande | null = null;
	readonly #max: number = 5;
	#fruits: Fruit[];

	private constructor() {
		this.#fruits = [];
	}

	public static getInstance(): Commande {
		if (Commande.#instance === null) {
			Commande.#instance = new Commande();
		}

		return Commande.#instance;
	}

	get size(): number {
		return this.#fruits.length;
	}

	get max(): number {
		return this.#max;
	}

	get isFull(): boolean {
		return this.size === this.#max;
	}

	public ajouterFruit(fruit: Fruit) {
		if (this.isFull) {
			throw new Error("Commande est pleine!");
		}

		this.#fruits.push(fruit);
	}

	public afficher() {
		this.#fruits.forEach((fruit) => {
			console.log(`${fruit} - calories: ${fruit.getCalories()}`);
		});
	}
}
