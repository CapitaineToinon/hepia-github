import { Fruit } from "../Fruits/Fruit";
import { Pending } from "./State/Pending";
import { State } from "./State/State";

export class Commande {
	static #instance: Commande | null = null;
	readonly #max: number = 2;
	#fruits: Fruit[];
	#state: State;

	private constructor() {
		this.#fruits = [];
		this.#state = new Pending();
	}

	public static getInstance(): Commande {
		if (Commande.#instance === null) {
			Commande.#instance = new Commande();
		}

		return Commande.#instance;
	}

	get state(): State {
		return this.#state;
	}

	set state(state: State) {
		this.#state = state;
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
			console.log(fruit.toString());
		});
	}

	traiterCommande() {
		this.#state.traiterCommande(this);
	}
}
