import { Commande } from "../index";

export abstract class State {
	abstract traiterCommande(commande: Commande): void;
}
