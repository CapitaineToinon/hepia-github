import prompts from "prompts";
import { Commande } from "./Commande";
import { FruitEnrichi } from "./Fruits/FruitEnrichi";
import { FruitType } from "./Fruits/FruitType";
import { FruitPauvre } from "./Fruits/FruitPauvre";
import { FruitNormal } from "./Fruits/FruitNormal";
import { Banane } from "./Fruits/Banane";
import { Pomme } from "./Fruits/Pomme";
import { Fruit } from "./Fruits/Fruit";

type Answer = {
	fruit: Fruit;
};

export class Client {
	#commande: Commande;

	constructor() {
		this.#commande = Commande.getInstance();
		this.createCommande();
	}

	get progress(): string {
		return `${this.#commande.size}/${this.#commande.max}`;
	}

	async createCommande() {
		do {
			const { fruit } = (await prompts(
				[
					{
						type: "select",
						name: "fruitType",
						message: `Quel type de fruit voulez vous? (${this.progress})`,
						choices: [
							{ title: "riche", value: new FruitEnrichi() },
							{ title: "pauvre", value: new FruitPauvre() },
							{ title: "normal", value: new FruitNormal() },
						],
					},
					{
						type: "select",
						name: "fruit",
						message: `Quel fruit voulez vous? (${this.progress})`,
						choices: (type: FruitType) => [
							{ title: "banane", value: new Banane({ type }) },
							{ title: "pomme", value: new Pomme({ type }) },
						],
					},
				],
				{
					onCancel() {
						process.exit(1);
					},
				}
			)) as Answer;

			this.#commande.ajouterFruit(fruit);
			console.log("Fruit ajouté à la commande!");
		} while (!this.#commande.isFull);

		this.#commande.afficher();
	}
}
