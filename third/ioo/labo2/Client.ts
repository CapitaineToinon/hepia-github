import prompts from "prompts";
import { Commande } from "./Commande/index";
import { FruitEnrichi } from "./Fruits/FruitEnrichi";
import { FruitType } from "./Fruits/FruitType";
import { FruitPauvre } from "./Fruits/FruitPauvre";
import { FruitNormal } from "./Fruits/FruitNormal";
import { Banane } from "./Fruits/Banane";
import { Pomme } from "./Fruits/Pomme";
import { Fruit } from "./Fruits/Fruit";
import { LineDecorator } from "./Decorators/LineDecorator";
import { Panier } from "./Fruits/Panier";
import { Decorator } from "./Decorators/Decorator";

type Answer = {
	fruitType: FruitType;
	fruit: Fruit;
	decorator: Decorator;
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
			let { decorator } = (await prompts(
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
							{
								title: "panier",
								value: new Panier({
									childs: [
										new Banane({ type }),
										new Banane({ type }),
										new Pomme({ type }),
									],
								}),
							},
						],
					},
					{
						type: "select",
						name: "decorator",
						message: `Quel character voulez vous utiliser pour décorer votre fruit? (${this.progress})`,
						choices: (fruit: Fruit) => {
							console.log(fruit);
							return [
								{ title: "Aucun", value: fruit },
								{ title: "*", value: new LineDecorator({ fruit, char: "*" }) },
								{ title: "#", value: new LineDecorator({ fruit, char: "#" }) },
								{ title: "=", value: new LineDecorator({ fruit, char: "=" }) },
							];
						},
					},
				],
				{
					onCancel() {
						process.exit(1);
					},
				}
			)) as Answer;

			this.#commande.ajouterFruit(decorator);
			console.log("Fruit ajouté à la commande!");
		} while (!this.#commande.isFull);

		this.#commande.afficher();

		// traite la commande a few times
		this.#commande.traiterCommande();
		this.#commande.traiterCommande();
		this.#commande.traiterCommande();
		this.#commande.traiterCommande();
	}
}
