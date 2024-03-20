#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensors){
	Action action = actIDLE;

	if(sensors.reset) reset();

	last_action_effect();

	if(!has_bikini && sensors.terreno[0] == 'K') has_bikini = true;

	if(!has_sneakers && sensors.terreno[0] == 'D') has_sneakers = true;

	if((!know_my_position && sensors.terreno[0] == 'G') || sensors.nivel == 0) start_exploration(sensors);

	if(know_my_position) update_map(sensors);

	check_if_charging_mode(sensors);


	if(have_pending_action){
		
		action = pending_action;
		have_pending_action = false;

	}else{

		set_target_position(sensors);
		
		if(has_target_position()){
			action = advance_to_target_position(target_position, sensors);
		}else{
			action = random_movement(sensors);
		}

	}


	print_debug_sensors_and_agent_data(sensors);	
	last_action = action;
	return action;
}


Action ComportamientoJugador::act_turn_sl(){
	pending_action = actTURN_SR;
	have_pending_action = true;
	return actTURN_L;
}


Action ComportamientoJugador::act_turn_r(){
	pending_action = actTURN_SR;
	have_pending_action = true;
	return actTURN_SR;
}


Action ComportamientoJugador::random_movement(Sensores sensors){
	Action action = actIDLE;
	int random = rand()%10;

	switch(random){
		case 0:
			action = actTURN_L;
			break;

		case 1:
			action = actTURN_SR;
			break;

		default:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				if(is_traversable_position(6, sensors)){
					action = actRUN;
				}
			}else{
				action = actTURN_L;
			}
				
			break;
	}

	return action;
}


void ComportamientoJugador::reset(){
	last_action = actIDLE;
	current_state.row = current_state.col = 99;
	current_state.compass = norte;
	know_my_position = false;
	has_sneakers = false;
	has_bikini = false;
	target_position = NULL_POSITION;
	recharge_position = NULL_POSITION;
	orientation_position = NULL_POSITION;
	bikini_position = NULL_POSITION;
	sneakers_position = NULL_POSITION;
	passageway_position = NULL_POSITION;
	battery_before_recharge = 0;
	have_pending_action = false;
    pending_action = actIDLE;

	num_resets++;
}


void ComportamientoJugador::last_action_effect(){
	switch(last_action){
		
		case actWALK:
			switch(current_state.compass){
				case norte:
					current_state.row--;
					break;

				case noreste:
					current_state.row--;
					current_state.col++;
					break;

				case este:
					current_state.col++;
					break;

				case sureste:
					current_state.row++;
					current_state.col++;
					break;

				case sur:
					current_state.row++;
					break;

				case suroeste:
					current_state.row++;
					current_state.col--;
					break;

				case oeste:
					current_state.col--;
					break;

				case noroeste:
					current_state.row--;
					current_state.col--;
					break;
			}
			break;

		case actRUN:
			switch(current_state.compass){
				case norte:
					current_state.row-=2;
					break;

				case noreste:
					current_state.row-=2;
					current_state.col+=2;
					break;

				case este:
					current_state.col+=2;
					break;

				case sureste:
					current_state.row+=2;
					current_state.col+=2;
					break;

				case sur:
					current_state.row+=2;
					break;

				case suroeste:
					current_state.row+=2;
					current_state.col-=2;
					break;

				case oeste:
					current_state.col-=2;
					break;

				case noroeste:
					current_state.row-=2;
					current_state.col-=2;
					break;
			}
			break;
		
		case actTURN_L:
			current_state.compass = static_cast<Orientacion> ((current_state.compass+6)%8);
			break;
		
		case actTURN_SR:
			current_state.compass = static_cast<Orientacion> ((current_state.compass+1)%8);
			break;

		default:			
			break;
	}
}


void ComportamientoJugador::start_exploration(Sensores sensors){
	current_state.row = sensors.posF;
	current_state.col = sensors.posC;
	current_state.compass = sensors.sentido;
	know_my_position = true;

	add_cliff_edge_to_map(sensors);
	update_map(sensors);
}


void ComportamientoJugador::update_map(Sensores sensors){
	int k = 1;
	int row = current_state.row;
	int col = current_state.col;
	
	switch(current_state.compass){
		
		case norte:
			mapaResultado[row][col] = sensors.terreno[0];
			for(int i=1; i<=3; i++){
				for(int j=i; j>=-i; j--){
					mapaResultado[row-i][col-j] = sensors.terreno[k];
					k++;
				}
			}
			break;

		
		case noreste:
			mapaResultado[row][col] = sensors.terreno[0];
			mapaResultado[row-1][col] = sensors.terreno[1];
			mapaResultado[row-1][col+1] = sensors.terreno[2];
			mapaResultado[row][col+1] = sensors.terreno[3];
			mapaResultado[row-2][col] = sensors.terreno[4];
			mapaResultado[row-2][col+1] = sensors.terreno[5];
			mapaResultado[row-2][col+2] = sensors.terreno[6];
			mapaResultado[row-1][col+2] = sensors.terreno[7];
			mapaResultado[row][col+2] = sensors.terreno[8];
			mapaResultado[row-3][col] = sensors.terreno[9];
			mapaResultado[row-3][col+1] = sensors.terreno[10];
			mapaResultado[row-3][col+2] = sensors.terreno[11];
			mapaResultado[row-3][col+3] = sensors.terreno[12];
			mapaResultado[row-2][col+3] = sensors.terreno[13];
			mapaResultado[row-1][col+3] = sensors.terreno[14];
			mapaResultado[row][col+3] = sensors.terreno[15];
			
			break;


		case este:
			mapaResultado[row][col] = sensors.terreno[0];
			for(int i=1; i<=3; i++){
				for(int j=i; j>=-i; j--){
					mapaResultado[row-j][col+i] = sensors.terreno[k];
					k++;
				}
			}
			break;
		
		case sureste:
			mapaResultado[row][col] = sensors.terreno[0];
			mapaResultado[row][col+1] = sensors.terreno[1];
			mapaResultado[row+1][col+1] = sensors.terreno[2];
			mapaResultado[row+1][col] = sensors.terreno[3];
			mapaResultado[row][col+2] = sensors.terreno[4];
			mapaResultado[row+1][col+2] = sensors.terreno[5];
			mapaResultado[row+2][col+2] = sensors.terreno[6];
			mapaResultado[row+2][col+1] = sensors.terreno[7];
			mapaResultado[row+2][col] = sensors.terreno[8];
			mapaResultado[row][col+3] = sensors.terreno[9];
			mapaResultado[row+1][col+3] = sensors.terreno[10];
			mapaResultado[row+2][col+3] = sensors.terreno[11];
			mapaResultado[row+3][col+3] = sensors.terreno[12];
			mapaResultado[row+3][col+2] = sensors.terreno[13];
			mapaResultado[row+3][col+1] = sensors.terreno[14];
			mapaResultado[row+3][col] = sensors.terreno[15];
			
			break;


		case sur:
			mapaResultado[row][col] = sensors.terreno[0];
			for(int i=1; i<=3; i++){
				for(int j=i; j>=-i; j--){
					mapaResultado[row+i][col+j] = sensors.terreno[k];
					k++;
				}
			}
			break;
		

		case suroeste:
			mapaResultado[row][col] = sensors.terreno[0];
			mapaResultado[row+1][col] = sensors.terreno[1];
			mapaResultado[row+1][col-1] = sensors.terreno[2];
			mapaResultado[row][col-1] = sensors.terreno[3];
			mapaResultado[row+2][col] = sensors.terreno[4];
			mapaResultado[row+2][col-1] = sensors.terreno[5];
			mapaResultado[row+2][col-2] = sensors.terreno[6];
			mapaResultado[row+1][col-2] = sensors.terreno[7];
			mapaResultado[row][col-2] = sensors.terreno[8];
			mapaResultado[row+3][col] = sensors.terreno[9];
			mapaResultado[row+3][col-1] = sensors.terreno[10];
			mapaResultado[row+3][col-2] = sensors.terreno[11];
			mapaResultado[row+3][col-3] = sensors.terreno[12];
			mapaResultado[row+2][col-3] = sensors.terreno[13];
			mapaResultado[row+1][col-3] = sensors.terreno[14];
			mapaResultado[row][col-3] = sensors.terreno[15];
			
			break;
			
		
		case oeste:
			mapaResultado[row][col] = sensors.terreno[0];
			for(int i=1; i<=3; i++){
				for(int j=i; j>=-i; j--){
					mapaResultado[row+j][col-i] = sensors.terreno[k];
					k++;
				}
			}
			break;

		case noroeste:
			mapaResultado[row][col] = sensors.terreno[0];
			mapaResultado[row][col-1] = sensors.terreno[1];
			mapaResultado[row-1][col-1] = sensors.terreno[2];
			mapaResultado[row-1][col] = sensors.terreno[3];
			mapaResultado[row][col-2] = sensors.terreno[4];
			mapaResultado[row-1][col-2] = sensors.terreno[5];
			mapaResultado[row-2][col-2] = sensors.terreno[6];
			mapaResultado[row-2][col-1] = sensors.terreno[7];
			mapaResultado[row-2][col] = sensors.terreno[8];
			mapaResultado[row][col-3] = sensors.terreno[9];
			mapaResultado[row-1][col-3] = sensors.terreno[10];
			mapaResultado[row-2][col-3] = sensors.terreno[11];
			mapaResultado[row-3][col-3] = sensors.terreno[12];
			mapaResultado[row-3][col-2] = sensors.terreno[13];
			mapaResultado[row-3][col-1] = sensors.terreno[14];
			mapaResultado[row-3][col] = sensors.terreno[15];
			
			break;
		
	}
}


void ComportamientoJugador::add_cliff_edge_to_map(Sensores sensors){
	//All maps are  square and surrounded by a x3 precipice border.
	int border_size = 3;
	
	int map_size = mapaResultado.size();

	//3 first rows
	for(int i = 0; i < border_size; i++){
		for(int j = 0; j < map_size; j++){
			mapaResultado[i][j] = 'P';
		}
	}

	//3 last rows
	for(int i = map_size-border_size; i < map_size; i++){
		for(int j = 0; j < map_size; j++){
			mapaResultado[i][j] = 'P';
		}
	}

	//3 first and last collumns
	for(int i = border_size; i < map_size-border_size; i++){
		for(int j = 0; j < border_size; j++){
			mapaResultado[i][j] = 'P';
		}

		for(int k = map_size-border_size; k < map_size; k++){
			mapaResultado[i][k] = 'P';
		}
	}
}


void ComportamientoJugador::identify_interesting_positions(Sensores sensors){
	
	target_position = 
	recharge_position = 
	orientation_position = 
	bikini_position = 
	sneakers_position = 
	passageway_position = NULL_POSITION;

	
	for(int i=1; i<sensors.terreno.size(); i++){
		
		switch (sensors.terreno[i])
		{
		case 'X':
			recharge_position = i;
			break;

		case 'G':
			if(!know_my_position){
				orientation_position = i;
			}
			break;

		case 'K':
			if(!has_bikini){
				bikini_position = i;
			}
			break;

		case 'D':
			if(!has_sneakers){
				sneakers_position = i;
			}
			break;
		
		default:
			break;
		}

	}

	passageway_position = search_passageways(sensors);
}


bool ComportamientoJugador::has_target_position(){
	return target_position != NULL_POSITION;
}


void ComportamientoJugador::set_target_position(Sensores sensors){

	identify_interesting_positions(sensors);

	//If see several interesting positions, select the one that seems to
	//have the highest priority as the target. 
	if(recharge_position != NULL_POSITION && sensors.bateria < MINIMUM_BATTERY){
		battery_before_recharge = sensors.bateria;
		target_position = recharge_position;
	}else{
		if(orientation_position != NULL_POSITION){
			target_position = orientation_position;
		}else{
			if(bikini_position != NULL_POSITION){
				target_position = bikini_position;
			}else{
				if(sneakers_position != NULL_POSITION){
					target_position = sneakers_position;
				}else{
					if(passageway_position != NULL_POSITION){
						target_position = passageway_position;
					}
				}
			}
		}
	}
}


int ComportamientoJugador::search_passageways(Sensores sensors){
	
	int passageway_position = NULL_POSITION;

	if(is_traversable_position(2, sensors)){	

		if((sensors.terreno[1] == 'M' || sensors.terreno[1] == 'P') && 
		(sensors.terreno[11] == 'M' || sensors.terreno[11] == 'P') &&
		is_traversable_position(5, sensors)){
				
			passageway_position = 5;
		}
		
		if((sensors.terreno[3] == 'M' || sensors.terreno[3] == 'P') && 
			(sensors.terreno[13] == 'M' || sensors.terreno[13] == 'P') &&
			is_traversable_position(7, sensors)){
			
			passageway_position = 7;
		}

		if( (sensors.terreno[5] == 'M' || sensors.terreno[5] == 'P') && 
		(sensors.terreno[7] == 'M' || sensors.terreno[7] == 'P') &&
		is_traversable_position(6, sensors)){
		
			passageway_position = 6;
		}

		if((sensors.terreno[1] == 'M' || sensors.terreno[1] == 'P') && 
		(sensors.terreno[3] == 'M' || sensors.terreno[3] == 'P')){
			passageway_position = 2;
		}
	}

	return passageway_position;
}


Action ComportamientoJugador::advance_to_target_position(int target_position, Sensores sensors){
	Action action = actIDLE;

	switch(target_position){

		case 1:
			if(is_traversable_position(1, sensors)){
				action = act_turn_sl();
				target_position = 2;
			}							
			break;

		case 2:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = NULL_POSITION;
			}
			break;

		case 3:
			if(is_traversable_position(3, sensors)){
				action = actTURN_SR;
				target_position = 2;
			}
			break;

		case 4:
			if(is_traversable_position(1, sensors)){
				action = act_turn_sl();
				target_position = 6;
			}
			break;

		case 5:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 1;
			}else{
				if(is_traversable_position(1, sensors)){
					action = act_turn_sl();
					target_position = 7;
				}else{
					target_position = NULL_POSITION;
				}
			}
			break;

		case 6:
			if(is_traversable_position(2, sensors) &&
			is_traversable_position(6, sensors)){
				action = actRUN;
				target_position = NULL_POSITION;
			}
			break;

		case 7:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 3;
			}
			break;

		case 8:
			if(is_traversable_position(3, sensors)){
				action = actTURN_SR;
				target_position = 6;
			}
			break;

		case 9:
			if(is_traversable_position(1, sensors)){
				action = act_turn_sl();
				target_position = 12;
			}
			break;

		case 10:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 4;
			}
			break;

		case 11:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 5;
			}
			break;

		case 12:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 6;
			}
			break;

		case 13:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 7;
			}
			break;

		case 14:
			if(is_traversable_position(2, sensors)){
				action = actWALK;
				target_position = 8;
			}
			break;

		case 15:
			if(is_traversable_position(3, sensors)){
				action = actTURN_SR;
				target_position = 12;
			}
			break;

	}

	return action;
}


bool ComportamientoJugador::is_traversable_position(int position, Sensores sensors){
	return (sensors.terreno[position] != 'P' && 
			sensors.terreno[position] != 'M' &&
			sensors.agentes[position] == '_');
}


void ComportamientoJugador::check_if_charging_mode(Sensores sensors){
	if(sensors.bateria < battery_before_recharge+MAXIMUN_RECHARGE && sensors.terreno[0] == 'X'){
		have_pending_action = true;
		pending_action = actIDLE;
	}
}


void ComportamientoJugador::print_debug_sensors_and_agent_data(Sensores sensors){
	cout << endl;
	cout << "Colisión: " << sensors.colision << endl;
	cout << "Reset: " << sensors.reset << endl;
	cout << "Numero reinicios: " << num_resets << endl;
	cout << "Vida: " << sensors.vida << endl;
	cout << "Bateria: " << sensors.bateria << endl;
	cout << "Tiempo: " << sensors.tiempo << endl;
	cout << "Tamanio mapa: " << mapaResultado.size() << endl;
	cout << endl;

	cout << "Posicion en GUI: fila " << sensors.posF << " columna " << sensors.posC << " ";
	switch(sensors.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este " << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur" << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste " << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}

	cout << "Posicion actual: fila " << current_state.row << " columna " << current_state.col << " ";
	switch(current_state.compass){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este " << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur" << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste " << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensors.terreno.size(); i++)
		cout << sensors.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensors.agentes.size(); i++)
		cout << sensors.agentes[i];
	cout << endl;
	cout << endl;

	cout << "Ultima Acci: ";
	switch (last_action){
	case actIDLE: cout << "actIDLE" << endl; break;
	case actTURN_L: cout << "actTURN_L" << endl; break;
	case actTURN_SR: cout << "actTURN_SR" << endl; break;
	case actWALK: cout << "actWALK" << endl; break;
	case actRUN: cout << "actRUN" << endl; break;	
	}
	
	cout << "Casilla objetivo: " << target_position << endl;
	cout << endl;

	//cout << "Inicio partida: " << inicio_partida << endl;
	//cout << "Bien situado: " << bien_situado << endl;
	//cout << "Frente no explorado: " << frente_no_exp << endl;
	cout << "Zapatillas: " << has_sneakers << endl;
	cout << "Bikini: " << has_bikini << endl;
	cout << endl;

	cout << endl;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}