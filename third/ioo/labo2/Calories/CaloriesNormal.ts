import { Fruit } from "../Fruits/Fruit";

export class CaloriesNormal {
	static #table = new Map<string, number>([
		["pomme", 200],
		["banane", 300],
	]);

	public static getCal(fruit: Fruit): number {
		return CaloriesNormal.#table.get(fruit.nom);
	}
}
