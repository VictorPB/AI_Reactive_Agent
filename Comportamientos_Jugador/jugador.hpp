#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
  int row;
  int col;
  Orientacion compass;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      num_resets = 0;
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
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
    int num_resets;
    Action last_action;
    state current_state;
    bool know_my_position;
    bool has_sneakers;
    bool has_bikini;
    int target_position;
    const int NULL_POSITION = -1;
    const int MAXIMUN_RECHARGE = 300;
    const int MINIMUM_BATTERY = 1000;
    int battery_before_recharge;
    bool have_pending_action;
    Action pending_action;
    int recharge_position;
    int orientation_position;
    int bikini_position;
    int sneakers_position;
    int passageway_position;


    Action act_turn_sl();

    Action act_turn_r();

    Action random_movement(Sensores sensors);
 
    void reset();

    void last_action_effect();

    void start_exploration(Sensores sensors);

    void update_map(Sensores sensors);

    void add_cliff_edge_to_map(Sensores sensors);

    void identify_interesting_positions(Sensores sensors);
    
    bool has_target_position();

    void set_target_position(Sensores sensors);

    int search_passageways(Sensores sensors);

    Action advance_to_target_position(int target_position, Sensores sensors);

    bool is_traversable_position(int position, Sensores sensors);
    
    void check_if_charging_mode(Sensores sensors);

    void print_debug_sensors_and_agent_data(Sensores sensors);

    
};
#endif
