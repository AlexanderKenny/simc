// ==========================================================================
// Dedmonwakeen's DPS-DPM Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simulationcraft.hpp"

namespace { // ANONYMOUS NAMESPACE

// The purpose of these namespaces is to allow modern IDEs to collapse sections of code.
// Is neither intended nor desired to provide name-uniqueness, hence the global uplift.

namespace spells {}
namespace attacks  {}
namespace pet_actions  {}

using namespace attacks;
using namespace spells;
using namespace pet_actions;

// ==========================================================================
// Hunter
// ==========================================================================

#define CROW_LIMIT 8

struct hunter_t;

struct hunter_pet_t;

enum aspect_type { ASPECT_NONE=0, ASPECT_HAWK, ASPECT_FOX, ASPECT_MAX };

struct hunter_td_t : public actor_pair_t
{
  struct dots_t
  {
    dot_t* serpent_sting;
  } dots;

  hunter_td_t( player_t* target, hunter_t* p );
};

struct hunter_t : public player_t
{
public:
  // Active
  hunter_pet_t* active_pet;
  aspect_type   active_aspect;
  action_t*     active_piercing_shots;
  action_t*     active_vishanka;

  // Secondary pets
  pet_t* moc_crows[ CROW_LIMIT ];

  // Buffs
  struct buffs_t
  {
    buff_t* aspect_of_the_hawk;
    buff_t* beast_within;
    buff_t* bombardment;
    buff_t* cobra_strikes;
    buff_t* focus_fire;
    buff_t* steady_focus;
    buff_t* lock_and_load;
    buff_t* master_marksman;
    buff_t* master_marksman_fire;
    buff_t* pre_steady_focus;
    buff_t* rapid_fire;
    // buff_t* trueshot_aura;
    buff_t* tier13_4pc;
  } buffs;

  // Cooldowns
  struct cooldowns_t
  {
    cooldown_t* explosive_shot;
    cooldown_t* viper_venom;
    cooldown_t* vishanka;
  } cooldowns;

  // Custom Parameters
  std::string summon_pet_str;

  // Gains
  struct gains_t
  {
    gain_t* invigoration;
    gain_t* fervor;
    gain_t* focus_fire;
    gain_t* rapid_recuperation;
    gain_t* thrill_of_the_hunt;
    gain_t* steady_shot;
    gain_t* cobra_shot;
    gain_t* viper_venom;
  } gains;

  // Procs
  struct procs_t
  {
    proc_t* invigoration;
    proc_t* thrill_of_the_hunt;
    proc_t* wild_quiver;
    proc_t* lock_and_load;
    proc_t* explosive_shot_focus_starved;
    proc_t* black_arrow_focus_starved;
  } procs;

  // Random Number Generation
  struct rngs_t
  {
    rng_t* frenzy;
    rng_t* invigoration;
    rng_t* rabid_power;
    rng_t* thrill_of_the_hunt;
  } rngs;

  // Talents
  struct talents_t
  {
    const spell_data_t* posthaste;
    const spell_data_t* narrow_escape;
    const spell_data_t* exhilaration;

    const spell_data_t* silencing_shot;
    const spell_data_t* wyvern_sting;
    const spell_data_t* binding_shot;

    const spell_data_t* crouching_tiger_hidden_chimera;
    const spell_data_t* aspect_of_the_iron_hawk;
    const spell_data_t* spirit_bond;

    const spell_data_t* fervor;
    const spell_data_t* readiness; // check
    const spell_data_t* thrill_of_the_hunt;

    const spell_data_t* a_murder_of_crows;
    const spell_data_t* dire_beast;
    const spell_data_t* lynx_rush;

    const spell_data_t* glaive_toss;
    const spell_data_t* powershot;
    const spell_data_t* barrage;
  } talents;

  // Specialization Spells
  struct specs_t
  {
    // Baseline
    // const spell_data_t* trueshot_aura;

    // Shared
    //const spell_data_t* cobra_shot;

    // Beast Mastery
    // const spell_data_t* kill_command;
    // const spell_data_t* intimidation;
    const spell_data_t* go_for_the_throat;
    const spell_data_t* beast_cleave;
    const spell_data_t* frenzy;
    const spell_data_t* focus_fire;
    // const spell_data_t* bestial_wrath;
    const spell_data_t* cobra_strikes;
    const spell_data_t* the_beast_within;
    // const spell_data_t* kindred_spirits;
    const spell_data_t* invigoration;
    const spell_data_t* exotic_beasts;
    //
    // // Marksmanship
    // const spell_data_t* aimed_shot;
    const spell_data_t* careful_aim;
    // const spell_data_t* concussive_barrage;
    const spell_data_t* bombardment;
    const spell_data_t* rapid_recuperation;
    const spell_data_t* master_marksman;
    // const spell_data_t* chimera_shot;
    const spell_data_t* steady_focus;
    const spell_data_t* piercing_shots;
    // // const spell_data_t* wild quiver;
    //
    // // Survival
    // const spell_data_t* explosive_shot;
    // const spell_data_t* lock_and_load;
    const spell_data_t* improved_serpent_sting;
    // const spell_data_t* black_arrow;
    // const spell_data_t* entrapment;
    const spell_data_t* viper_venom;
    const spell_data_t* trap_mastery;
    const spell_data_t* serpent_spread;
  } specs;

  // Glyphs
  struct glyphs_t
  {
    // Major
    const spell_data_t* aimed_shot;
    const spell_data_t* animal_bond;
    const spell_data_t* black_ice;
    const spell_data_t* camouflage;
    const spell_data_t* chimera_shot;
    const spell_data_t* deterrence;
    const spell_data_t* disengage;
    const spell_data_t* distracting_shot;
    const spell_data_t* endless_wrath;
    const spell_data_t* explosive_trap;
    const spell_data_t* freezing_trap;
    const spell_data_t* ice_trap;
    const spell_data_t* icy_solace;
    const spell_data_t* marked_for_death;
    const spell_data_t* masters_call;
    const spell_data_t* mend_pet;
    const spell_data_t* mending;
    const spell_data_t* mirrored_blades;
    const spell_data_t* misdirection;
    const spell_data_t* no_escape;
    const spell_data_t* pathfinding;
    const spell_data_t* scatter_shot;
    const spell_data_t* scattering;
    const spell_data_t* snake_trap;
    const spell_data_t* tranquilizing_shot;

    // Minor
    const spell_data_t* aspects;
    const spell_data_t* aspect_of_the_pack;
    const spell_data_t* direction;
    const spell_data_t* fetch;
    const spell_data_t* fireworks;
    const spell_data_t* lesser_proportion;
    const spell_data_t* marking;
    const spell_data_t* revive_pet;
    const spell_data_t* stampede;
    const spell_data_t* tame_beast;
    const spell_data_t* the_cheetah;
  } glyphs;

  struct mastery_spells_t
  {
    const spell_data_t* master_of_beasts;
    const spell_data_t* wild_quiver;
    const spell_data_t* essence_of_the_viper;
  } mastery;
private:
  target_specific_t<hunter_td_t> target_data;
public:
  double merge_piercing_shots;
  double tier13_4pc_cooldown;
  uint32_t vishanka;

  hunter_t( sim_t* sim, const std::string& name, race_e r = RACE_NONE ) :
    player_t( sim, HUNTER, name, r == RACE_NONE ? RACE_NIGHT_ELF : r ),
    buffs( buffs_t() ),
    cooldowns( cooldowns_t() ),
    gains( gains_t() ),
    procs( procs_t() ),
    rngs( rngs_t() ),
    talents( talents_t() ),
    specs( specs_t() ),
    glyphs( glyphs_t() ),
    mastery( mastery_spells_t() )
  {
    target_data.init( "target_data", this );

    // Active
    active_pet             = 0;
    active_aspect          = ASPECT_NONE;
    active_piercing_shots  = 0;
    active_vishanka        = 0;

    merge_piercing_shots = 0;

    // Cooldowns
    cooldowns.explosive_shot = get_cooldown( "explosive_shot" );
    cooldowns.viper_venom    = get_cooldown( "viper_venom" );
    cooldowns.vishanka       = get_cooldown( "vishanka"       );

    summon_pet_str = "";
    initial.distance = 40;
    base_gcd = timespan_t::from_seconds( 1.0 );

    tier13_4pc_cooldown = 105.0;
    vishanka = 0;
  }

  // Character Definition
  virtual void      init_spells();
  virtual void      init_base();
  virtual void      init_buffs();
  virtual void      init_values();
  virtual void      init_gains();
  virtual void      init_position();
  virtual void      init_procs();
  virtual void      init_rng();
  virtual void      init_scaling();
  virtual void      init_actions();
  virtual void      init_items();
  virtual void      register_callbacks();
  virtual void      combat_begin();
  virtual void      reset();
  virtual double    composite_attack_power_multiplier();
  virtual double    composite_attack_haste();
  virtual double    composite_player_multiplier( school_e school, action_t* a = NULL );
  virtual double    matching_gear_multiplier( attribute_e attr );
  virtual void      create_options();
  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual pet_t*    create_pet( const std::string& name, const std::string& type = std::string() );
  virtual void      create_pets();
  virtual int       decode_set( item_t& );
  virtual resource_e primary_resource() { return RESOURCE_FOCUS; }
  virtual role_e primary_role() { return ROLE_ATTACK; }
  virtual bool      create_profile( std::string& profile_str, save_e=SAVE_ALL, bool save_html=false );
  virtual void      copy_from( player_t* source );
  virtual void      armory_extensions( const std::string& r, const std::string& s, const std::string& c, cache::behavior_e );
  virtual void      moving();

  virtual hunter_td_t* get_target_data( player_t* target )
  {
    hunter_td_t*& td = target_data[ target ];
    if ( ! td ) td = new hunter_td_t( target, this );
    return td;
  }

  // Event Tracking
  virtual void regen( timespan_t periodicity );

  // Temporary
  virtual std::string set_default_talents();
  virtual std::string set_default_glyphs();
};

// ==========================================================================
// Hunter Pet
// ==========================================================================

struct hunter_pet_td_t : public actor_pair_t
{
  hunter_pet_td_t( player_t* target, hunter_pet_t* p );
};

struct hunter_pet_t : public pet_t
{
public:
  action_t* kill_command;

  struct talents_t
  {
    // Common Talents
    const spell_data_t* dash;

    // Cunning
    const spell_data_t* roar_of_sacrifice;
    const spell_data_t* bullheaded;
    // const spell_data_t* feeding_frenzy;
    const spell_data_t* cornered;
    const spell_data_t* boars_speed;

    // Ferocity
    const spell_data_t* rabid;
    const spell_data_t* heart_of_the_phoenix;
    const spell_data_t* spiked_collar;

    // Tenacity
    const spell_data_t* last_stand;
    const spell_data_t* charge;
    const spell_data_t* blood_of_the_rhino;
    const spell_data_t* great_stamina;
    const spell_data_t* thunderstomp;

  } talents;

  struct specs_t
  {
    const spell_data_t* wild_hunt;
  } specs;

  // Buffs
  struct buffs_t
  {
    buff_t* bestial_wrath;
    buff_t* frenzy;
    buff_t* rabid;
    buff_t* rabid_power_stack;
  } buffs;

  // Gains
  struct gains_t
  {
    gain_t* fervor;
    gain_t* focus_fire;
    gain_t* go_for_the_throat;
  } gains;

  // Benefits
  struct benefits_t
  {
    benefit_t* wild_hunt;
  } benefits;

private:
  target_specific_t<hunter_pet_td_t> target_data;
public:
  hunter_pet_t( sim_t* sim, hunter_t* owner, const std::string& pet_name, pet_e pt ) :
    pet_t( sim, owner, pet_name, pt ),
    talents( talents_t() ),
    specs( specs_t() ),
    buffs( buffs_t() ),
    gains( gains_t() ),
    benefits( benefits_t() )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = rating_t::interpolate( level, 0, 0, 51, 73 ); // FIXME needs level 60 and 70 values
    main_hand_weapon.max_dmg    = rating_t::interpolate( level, 0, 0, 78, 110 ); // FIXME needs level 60 and 70 values
    // Level 85 numbers from Rivkah from EJ, 07.08.2011
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = timespan_t::from_seconds( 2.0 );

    stamina_per_owner = 0.45;

    //health_per_stamina *= 1.05; // 3.1.0 change # Cunning, Ferocity and Tenacity pets now all have +5% damage, +5% armor and +5% health bonuses
    initial.armor_multiplier *= 1.05;

    target_data.init( "target_data", this );
  }

  hunter_t* cast_owner() const
  { return static_cast<hunter_t*>( owner ); }

  virtual pet_e group()
  {
    //assert( pet_type > PET_NONE && pet_type < PET_HUNTER );
    if ( pet_type > PET_NONE          && pet_type < PET_FEROCITY_TYPE ) return PET_FEROCITY_TYPE;
    if ( pet_type > PET_FEROCITY_TYPE && pet_type < PET_TENACITY_TYPE ) return PET_TENACITY_TYPE;
    if ( pet_type > PET_TENACITY_TYPE && pet_type < PET_CUNNING_TYPE  ) return PET_CUNNING_TYPE;
    return PET_NONE;
  }

  virtual const char* unique_special()
  {
    switch ( pet_type )
    {
    case PET_CARRION_BIRD: return "demoralizing_screech";
    case PET_CAT:          return "roar_of_courage";
    case PET_CORE_HOUND:   return NULL;
    case PET_DEVILSAUR:    return "furious_howl/monstrous_bite";
    case PET_HYENA:        return "cackling_howl";
    case PET_MOTH:         return NULL;
    case PET_RAPTOR:       return "tear_armor";
    case PET_SPIRIT_BEAST: return "roar_of_courage";
    case PET_TALLSTRIDER:  return NULL;
    case PET_WASP:         return NULL;
    case PET_WOLF:         return "furious_howl";
    case PET_BEAR:         return NULL;
    case PET_BOAR:         return NULL;
    case PET_CRAB:         return NULL;
    case PET_CROCOLISK:    return NULL;
    case PET_GORILLA:      return NULL;
    case PET_RHINO:        return NULL;
    case PET_SCORPID:      return NULL;
    case PET_SHALE_SPIDER: return NULL;
    case PET_TURTLE:       return NULL;
    case PET_WARP_STALKER: return NULL;
    case PET_WORM:         return NULL;
    case PET_BAT:          return NULL;
    case PET_BIRD_OF_PREY: return NULL;
    case PET_CHIMERA:      return "froststorm_breath";
    case PET_DRAGONHAWK:   return "lightning_breath";
    case PET_NETHER_RAY:   return NULL;
    case PET_RAVAGER:      return "ravage";
    case PET_SERPENT:      return "corrosive_spit";
    case PET_SILITHID:     return "qiraji_fortitude";
    case PET_SPIDER:       return NULL;
    case PET_SPOREBAT:     return NULL;
    case PET_WIND_SERPENT: return "lightning_breath";
    case PET_FOX:          return "tailspin";
    default: break;
    }
    return NULL;
  }

  virtual bool ooc_buffs() { return true; }

  virtual void init_base()
  {
    pet_t::init_base();

    base.attribute[ ATTR_STRENGTH  ] = rating_t::interpolate( level, 0, 162, 331, 476 );
    base.attribute[ ATTR_AGILITY   ] = rating_t::interpolate( level, 0, 54, 113, 438 );
    base.attribute[ ATTR_STAMINA   ] = rating_t::interpolate( level, 0, 307, 361 ); // stamina is different for every pet type
    base.attribute[ ATTR_INTELLECT ] = 100; // FIXME: is 61 at lvl 75. Use /script print(UnitStats("pet",x)); 1=str,2=agi,3=stam,4=int,5=spi
    base.attribute[ ATTR_SPIRIT    ] = 100; // FIXME: is 101 at lvl 75. Values are equal for a cat and a gorilla.

    base.attack_crit = 0.05; // Assume 5% base crit as for most other pets. 19/10/2011

    resources.base[ RESOURCE_HEALTH ] = rating_t::interpolate( level, 0, 4253, 6373 );
    resources.base[ RESOURCE_FOCUS ] = 100 /*+ cast_owner() -> talents.kindred_spirits -> effectN( 1 ).resource( RESOURCE_FOCUS )*/;

    base_focus_regen_per_second  = ( 24.5 / 4.0 );

    base_gcd = timespan_t::from_seconds( 1.20 );

    resources.infinite_resource[ RESOURCE_FOCUS ] = cast_owner() -> resources.infinite_resource[ RESOURCE_FOCUS ];

    world_lag = timespan_t::from_seconds( 0.3 ); // Pet AI latency to get 3.3s claw cooldown as confirmed by Rivkah on EJ, August 2011
    world_lag_override = true;

    owner_coeff.ap_from_ap = 1.0;
    owner_coeff.sp_from_ap = 0.5;
  }

  virtual void init_talents()
  {
    // Common Talents
    talents.spiked_collar     = find_talent_spell( "Spiked Collar" );

    // Ferocity
    talents.rabid            = find_talent_spell( "Rabid" );

    // Cunning
    talents.roar_of_sacrifice = find_talent_spell( "Roar of Recovery" );

    pet_t::init_talents();
  }

  virtual void init_buffs()
  {
    pet_t::init_buffs();
    buffs.bestial_wrath     = buff_creator_t( this, 19574, "bestial_wrath" );
    buffs.frenzy            = buff_creator_t( this, 19615, "frenzy_effect" );
    buffs.rabid             = buff_creator_t( this, 53401, "rabid" );
    buffs.rabid_power_stack = buff_creator_t( this, 53403, "rabid_power_stack" );
  }

  virtual void init_gains()
  {
    pet_t::init_gains();

    gains.fervor            = get_gain( "fervor"            );
    gains.focus_fire        = get_gain( "focus_fire"        );
    gains.go_for_the_throat = get_gain( "go_for_the_throat" );
  }

  virtual void init_benefits()
  {
    pet_t::init_benefits();

    benefits.wild_hunt = get_benefit( "wild_hunt" );
  }
  virtual void init_actions()
  {
    if ( action_list_str.empty() )
    {
      action_list_str += "/auto_attack";
      action_list_str += "/snapshot_stats";

      if ( talents.rabid -> ok() )
      {
        action_list_str += "/rabid";
      }

      const char* special = unique_special();
      if ( special )
      {
        action_list_str += "/";
        action_list_str += special;
      }
      action_list_str += "/claw";
      action_list_str += "/wait_until_ready";
      action_list_default = 1;
    }

    pet_t::init_actions();
  }

  virtual double composite_attack_power_multiplier()
  {
    double mult = pet_t::composite_attack_power_multiplier();

    mult *= 1.0 + buffs.rabid_power_stack -> stack() * buffs.rabid_power_stack -> data().effectN( 1 ).percent();
    // TODO pet charge should show up here.
    return mult;
  }

  virtual double composite_attack_crit( weapon_t* /* w */ )
  {
    double ac = pet_t::composite_attack_crit();

    ac *= 1.0 + talents.spiked_collar -> effectN( 3 ).percent();

    return ac;
  }

  virtual double composite_attack_haste()
  {
    double h = pet_t::composite_attack_haste();

    h *= 1.0 + cast_owner() -> buffs.rapid_fire -> check() * cast_owner() -> buffs.rapid_fire -> current_value;

    return h;
  }

  virtual void summon( timespan_t duration=timespan_t::zero() )
  {
    pet_t::summon( duration );

    cast_owner() -> active_pet = this;
  }

  virtual void demise()
  {
    pet_t::demise();

    cast_owner() -> active_pet = 0;
  }

  virtual double composite_player_multiplier( school_e school, action_t* a )
  {
    double m = pet_t::composite_player_multiplier( school, a );

    if ( cast_owner() -> mastery.master_of_beasts -> ok() )
    {
      m *= 1.0 + cast_owner() -> mastery.master_of_beasts -> effectN( 1 ).mastery_value() * owner -> composite_mastery();
    }

    if ( buffs.bestial_wrath -> up() )
      m *= 1.0 + buffs.bestial_wrath -> data().effectN( 2 ).percent();

    // Orc racial
    if ( owner -> race == RACE_ORC )
      m *= 1.05;

    return m;
  }

  virtual hunter_pet_td_t* get_target_data( player_t* target )
  {
    hunter_pet_td_t*& td = target_data[ target ];
    if ( ! td ) td = new hunter_pet_td_t( target, this );
    return td;
  }

  virtual resource_e primary_resource() { return RESOURCE_FOCUS; }

  virtual action_t* create_action( const std::string& name, const std::string& options_str );

  virtual void init_spells();
};

// Template for common hunter action code. See priest_action_t.
template <class Base>
struct hunter_action_t : public Base
{
  typedef Base ab;
  typedef hunter_action_t base_t;

  hunter_action_t( const std::string& n, hunter_t* player,
                   const spell_data_t* s = spell_data_t::nil() ) :
    ab( n, player, s )
  {
    ab::stateless = true;
  }

  hunter_t* p() const { return static_cast<hunter_t*>( ab::player ); }

  hunter_td_t* cast_td( player_t* t = 0 ) { return p() -> get_target_data( t ? t : ab::target ); }

  virtual double cost()
  {
    double c = ab::cost();

    if ( c == 0 )
      return 0;

    if ( p() -> buffs.beast_within -> check() )
      c *= ( 1.0 + p() -> buffs.beast_within -> data().effectN( 1 ).percent() );

    return c;
  }

  // trigger_thrill_of_the_hunt ===============================================

  void trigger_thrill_of_the_hunt()
  {
    if ( ! p() -> talents.thrill_of_the_hunt -> ok() )
      return;

    if ( p() -> rngs.thrill_of_the_hunt -> roll ( p() -> talents.thrill_of_the_hunt -> proc_chance() ) )
    {
      double gain = ab::base_costs[ ab::current_resource() ] * p() -> talents.thrill_of_the_hunt -> effectN( 1 ).percent();
      p() -> procs.thrill_of_the_hunt -> occur();
      p() -> resource_gain( RESOURCE_FOCUS, gain, p() -> gains.thrill_of_the_hunt );
    }
  }
};

namespace attacks {

// ==========================================================================
// Hunter Attack
// ==========================================================================

struct hunter_ranged_attack_t : public hunter_action_t<ranged_attack_t>
{
  hunter_ranged_attack_t( const std::string& n, hunter_t* player,
                          const spell_data_t* s = spell_data_t::nil() ) :
    base_t( n, player, s )
  {
    may_crit               = true;
    tick_may_crit          = true;
    normalize_weapon_speed = true;
    dot_behavior           = DOT_REFRESH;
  }

  virtual void init()
  {
    if ( player -> main_hand_weapon.group() != WEAPON_RANGED )
    {
      background = true;
    }

    base_t::init();
  }

  virtual void trigger_steady_focus()
  {
    p() -> buffs.pre_steady_focus -> expire();
  }

  virtual void add_scope()
  {
    if ( player -> items[ weapon -> slot ].encoded_enchant_str == "scope" )
    {
      double scope_damage = util::ability_rank( player -> level, 15.0,72,  12.0,67,  7.0,0 );

      base_dd_min += scope_damage;
      base_dd_max += scope_damage;
    }
  }

  virtual void execute();

  virtual timespan_t execute_time()
  {
    timespan_t t = base_t::execute_time();

    if ( t == timespan_t::zero()  || base_execute_time < timespan_t::zero() )
      return timespan_t::zero();

    return t;
  }

  virtual double swing_haste()
  {
    double h = base_t::swing_haste();

    if ( p() -> buffs.steady_focus -> up() )
      h *= 1.0/ ( 1.0 + p() -> specs.steady_focus -> effectN( 1 ).percent() );

    return h;
  }

  virtual double action_multiplier()
  {
    double am = base_t::action_multiplier();
    return am;
  }

};

// trigger_go_for_the_throat ================================================

void trigger_go_for_the_throat( hunter_ranged_attack_t* a )
{
  if ( ! a -> p() -> specs.go_for_the_throat -> ok() )
    return;

  if ( ! a -> p() -> active_pet )
    return;

  a -> p() -> active_pet -> resource_gain( RESOURCE_FOCUS, a -> p() -> specs.go_for_the_throat -> effectN( 1 ).base_value(), a -> p() -> active_pet -> gains.go_for_the_throat );
}
struct piercing_shots_t : public ignite_like_action_t< attack_t, hunter_t >
{
  piercing_shots_t( hunter_t* p ) :
    base_t( "piercing_shots", p, p -> find_spell( 63468 ) )
  { }

};

// trigger_piercing_shots ===================================================

// Hunter Piercing Shots template specialization
template <class HUNTER_ACTION>
void trigger_piercing_shots( HUNTER_ACTION* s, player_t* t, double dmg )
{
  hunter_t* p = s -> p();
  if ( ! p -> specs.piercing_shots -> ok() ) return;

  trigger_ignite_like_mechanic(
    p -> active_piercing_shots, // ignite spell
    t, // target
    p -> specs.piercing_shots -> effectN( 1 ).percent() * dmg ); // dw damage
}

struct vishanka_t : public hunter_ranged_attack_t
{
  vishanka_t( hunter_t* p, uint32_t id ) :
    hunter_ranged_attack_t( "vishanka_jaws_of_the_earth", p, p -> find_spell( id ) )
  {
    background  = true;
    proc        = true;
    trigger_gcd = timespan_t::zero();
    init();

    // FIX ME: Can this crit, miss, etc?
    may_miss    = false;
  }
};
// trigger_vishanka =========================================================

static void trigger_vishanka( hunter_ranged_attack_t* a )
{
  if ( ! a -> p() -> vishanka )
    return;

  if ( a -> p() -> cooldowns.vishanka -> remains() > timespan_t::zero() )
    return;

  assert( a -> p() -> active_vishanka );

  if ( a -> sim -> roll( a -> p() -> dbc.spell( a -> p() -> vishanka ) -> proc_chance() ) )
  {
    a -> p() -> active_vishanka -> execute();
    a -> p() -> cooldowns.vishanka -> duration = timespan_t::from_seconds( 15.0 ); // Assume a ICD until testing proves one way or another
    a -> p() -> cooldowns.vishanka -> start();
  }
}

void hunter_ranged_attack_t::execute()
{
  ranged_attack_t::execute();

  if ( p() -> specs.steady_focus -> ok() )
    trigger_steady_focus();

  if ( p() -> buffs.pre_steady_focus -> stack() == 2 )
  {
    p() -> buffs.steady_focus -> trigger();
    p() -> buffs.pre_steady_focus -> expire();
  }

  trigger_thrill_of_the_hunt();

  if ( result_is_hit( execute_state -> result ) )
    trigger_vishanka( this );
}
// Ranged Attack ============================================================

struct ranged_t : public hunter_ranged_attack_t
{
  ranged_t( hunter_t* player, const char* name="ranged" ) :
    hunter_ranged_attack_t( name, player, spell_data_t::nil() /*, special true */ )
  {
    school = SCHOOL_PHYSICAL;
    weapon = &( player -> main_hand_weapon );
    base_execute_time = weapon -> swing_time;

    normalize_weapon_speed = false;
    may_crit    = true;
    background  = true;
    repeating   = true;
  }

  virtual timespan_t execute_time()
  {
    if ( ! player -> in_combat )
      return timespan_t::from_seconds( 0.01 );

    return hunter_ranged_attack_t::execute_time();
  }

  virtual void trigger_steady_focus()
  { }

  virtual void impact_s( action_state_t* s )
  {
    hunter_ranged_attack_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {
      if ( s -> result == RESULT_CRIT )
        trigger_go_for_the_throat( this );
    }
  }
};

// Auto Shot ================================================================

struct auto_shot_t : public hunter_ranged_attack_t
{
  auto_shot_t( hunter_t* p, const std::string& options_str ) :
    hunter_ranged_attack_t( "auto_shot", p )
  {
    parse_options( NULL, options_str );

    p -> main_hand_attack = new ranged_t( p );

    trigger_gcd = timespan_t::zero();
  }

  virtual void execute()
  {
    p() -> main_hand_attack -> schedule_execute();
  }

  virtual bool ready()
  {
    return( player -> main_hand_attack -> execute_event == 0 ); // not swinging
  }

  virtual timespan_t execute_time()
  {
    double h = 1.0;

    h *= 1.0 / ( 1.0 + sim -> auras.attack_haste -> value() );

    return hunter_ranged_attack_t::execute_time() * h;
  }
};

// Aimed Shot ===============================================================

struct aimed_shot_t : public hunter_ranged_attack_t
{

  // Aimed Shot - Master Marksman ===========================================

  struct aimed_shot_mm_t : public hunter_ranged_attack_t
  {
    aimed_shot_mm_t( hunter_t* p ) :
      hunter_ranged_attack_t( "aimed_shot_mm", p, p -> find_spell( 82928 ) )
    {
      check_spec ( HUNTER_MARKSMANSHIP );

      // Don't know why these values aren't 0 in the database.
      base_execute_time = timespan_t::zero();

      // Hotfix on Feb 18th, 2011: http://blue.mmo-champion.com/topic/157148/patch-406-hotfixes-february-18
      // Testing confirms that the weapon multiplier also affects the RAP coeff
      // and the base damage of the shot. Probably a bug on Blizzard's end.
      direct_power_mod  = 0.724; // hardcoded into tooltip
      direct_power_mod *= weapon_multiplier;

      base_dd_min *= weapon_multiplier; // Aimed Shot's weapon multiplier applies to the base damage as well
      base_dd_max *= weapon_multiplier;

      normalize_weapon_speed = true;
    }

    virtual double composite_target_crit( player_t* t )
    {
      double cc = hunter_ranged_attack_t::composite_target_crit( t );

      if ( p() -> specs.careful_aim -> ok() && t -> health_percentage() > p() -> specs.careful_aim -> effectN( 2 ).base_value() )
      {
        cc += p() -> specs.careful_aim -> effectN( 1 ).percent();
      }

      return cc;
    }

    virtual void execute()
    {
      hunter_ranged_attack_t::execute();

      p() -> buffs.master_marksman_fire -> expire();
    }

    virtual void impact_s( action_state_t* s )
    {
      hunter_ranged_attack_t::impact_s( s );

      if ( s -> result == RESULT_CRIT )
        trigger_piercing_shots( this, s -> target, s -> result_amount );
    }
  };

  aimed_shot_mm_t* as_mm;
  int casted;

  aimed_shot_t( hunter_t* p, const std::string& options_str ) :
    hunter_ranged_attack_t( "aimed_shot", p, p -> find_class_spell( "Aimed Shot" ) ),
    as_mm( 0 )
  {
    check_spec ( HUNTER_MARKSMANSHIP );
    parse_options( NULL, options_str );

    normalize_weapon_speed = true;
    base_dd_min *= weapon_multiplier; // Aimed Shot's weapon multiplier applies to the base damage as well
    base_dd_max *= weapon_multiplier;

    casted = 0;

    // Hotfix on Feb 18th, 2011: http://blue.mmo-champion.com/topic/157148/patch-406-hotfixes-february-18
    // Testing confirms that the weapon multiplier also affects the RAP coeff
    // and the base damage of the shot. Probably a bug on Blizzard's end.
    direct_power_mod  = 0.724; // hardcoded into tooltip
    direct_power_mod *= weapon_multiplier;

    as_mm = new aimed_shot_mm_t( p );
    as_mm -> background = true;
  }

  virtual double cost()
  {
    if ( p() -> buffs.master_marksman_fire -> check() )
      return 0;

    return hunter_ranged_attack_t::cost();
  }

  virtual timespan_t execute_time()
  {
    if ( p() -> buffs.master_marksman_fire -> check() )
      return timespan_t::zero();

    return hunter_ranged_attack_t::execute_time();
  }

  virtual void schedule_execute()
  {
    hunter_ranged_attack_t::schedule_execute();

    if ( time_to_execute > timespan_t::zero() )
    {
      p() -> main_hand_attack -> cancel();
      casted = 1;
    }
    else
    {
      casted = 0;
    }
  }

  virtual void execute()
  {
    if ( !casted )
    {
      as_mm -> execute();
    }
    else
    {
      hunter_ranged_attack_t::execute();
    }
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_ranged_attack_t::impact_s( s );

    if ( s -> result == RESULT_CRIT )
      trigger_piercing_shots( this, s -> target, s -> result_amount );
  }
};

// Arcane Shot Attack =======================================================

struct arcane_shot_t : public hunter_ranged_attack_t
{
  arcane_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "arcane_shot", player, player -> find_class_spell( "Arcane Shot" ) )
  {
    parse_options( NULL, options_str );

    direct_power_mod = 0.0483; // hardcoded into tooltip
  }

  virtual void impact_s( action_state_t* state )
  {
    hunter_ranged_attack_t::impact_s( state );
    if ( result_is_hit( state -> result ) )
    {
      p() -> buffs.cobra_strikes -> trigger( 2 );

      // Needs testing
      p() -> buffs.tier13_4pc -> trigger();
    }
  }
};

// Arcane Shot Attack =======================================================

struct power_shot_t : public hunter_ranged_attack_t
{
  power_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "power_shot", player, player -> find_class_spell( "Power Shot" ) )
  {
    parse_options( NULL, options_str );
  }

  virtual void impact_s( action_state_t* state )
  {
    hunter_ranged_attack_t::impact_s( state );
    if ( result_is_hit( state -> result ) )
    {
      p() -> buffs.cobra_strikes -> trigger( 2 );

      // Needs testing
      p() -> buffs.tier13_4pc -> trigger();
    }
  }
};

// Black Arrow ==============================================================

struct black_arrow_t : public hunter_ranged_attack_t
{
  black_arrow_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "black_arrow", player, player -> find_class_spell( "Black Arrow" ) )
  {
    parse_options( NULL, options_str );

    may_block  = false;
    may_crit   = false;

    cooldown = p() -> get_cooldown( "traps" );
    cooldown -> duration = data().cooldown();

    cooldown -> duration += p() -> specs.trap_mastery -> effectN( 4 ).time_value();
    base_multiplier *= 1.0 + p() -> specs.trap_mastery -> effectN( 2 ).percent();

    base_dd_min=base_dd_max=0;
    tick_power_mod = data().extra_coeff();
  }

  virtual bool ready()
  {
    if ( cooldown -> remains() == timespan_t::zero() && ! p() -> resource_available( RESOURCE_FOCUS, cost() ) )
    {
      if ( sim -> log ) sim -> output( "Player %s was focus starved when Black Arrow was ready.", p() -> name() );
      p() -> procs.black_arrow_focus_starved -> occur();
    }

    return hunter_ranged_attack_t::ready();
  }

  virtual void tick( dot_t* d )
  {
    hunter_ranged_attack_t::tick( d );

    if ( p() -> buffs.lock_and_load -> trigger( 2 ) )
    {
      p() -> procs.lock_and_load -> occur();
      p() -> cooldowns.explosive_shot -> reset();
    }
  }
};

// Explosive Trap ===========================================================

struct explosive_trap_effect_t : public hunter_ranged_attack_t
{
  explosive_trap_effect_t( hunter_t* player )
    : hunter_ranged_attack_t( "explosive_trap", player, player -> find_spell( 13812 ) )
  {
//    aoe = -1;
    background = true;
    tick_power_mod = data().extra_coeff();

    cooldown -> duration += p() -> specs.trap_mastery -> effectN( 4 ).time_value();
    base_multiplier *= 1.0 + p() -> specs.trap_mastery -> effectN( 2 ).percent();

    may_miss = false;
    may_crit = false;
    tick_may_crit = true;
  }

  virtual void tick( dot_t* d )
  {
    hunter_ranged_attack_t::tick( d );

    if ( p() -> buffs.lock_and_load -> trigger( 2 ) )
    {
      p() -> procs.lock_and_load -> occur();
      p() -> cooldowns.explosive_shot -> reset();
    }
  }
};

struct explosive_trap_t : public hunter_ranged_attack_t
{
  attack_t* trap_effect;
  int trap_launcher;

  explosive_trap_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "explosive_trap", player, player -> find_spell( 13813 ) ), trap_effect( 0 ),
    trap_launcher( 0 )
  {
    option_t options[] =
    {
      // Launched traps have a focus cost
      { "trap_launcher", OPT_BOOL, &trap_launcher },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    //TODO: Split traps cooldown into fire/frost/snakes
    cooldown = p() -> get_cooldown( "traps" );
    cooldown -> duration = data().cooldown();
//    cooldown -> duration += p() -> talents.resourcefulness -> effectN( 1 ).time_value();

    may_miss=false;

    trap_effect = new explosive_trap_effect_t( p() );
    add_child( trap_effect );
  }

  virtual void execute()
  {
    hunter_ranged_attack_t::execute();

    trap_effect -> execute();
  }

  virtual double cost()
  {
    if ( trap_launcher )
      return 20.0;

    return hunter_ranged_attack_t::cost();
  }
};

// Chimera Shot =============================================================

struct chimera_shot_t : public hunter_ranged_attack_t
{
  chimera_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "chimera_shot", player, player -> find_class_spell( "Chimera Shot" ) )
  {
    parse_options( NULL, options_str );

    direct_power_mod = 0.732; // hardcoded into tooltip

    normalize_weapon_speed = true;

    // FIXME
    //cooldown -> duration += p() -> glyphs.chimera_shot -> mod_additive_time( P_COOLDOWN );
  }


  virtual void execute()
  {
    hunter_ranged_attack_t::execute();

  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_ranged_attack_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {
      cast_td( s -> target ) -> dots.serpent_sting -> refresh_duration();

      if ( s -> result == RESULT_CRIT )
        trigger_piercing_shots( this, s -> target, s -> result_amount );

    }
  }
};

// Cobra Shot Attack ========================================================

struct cobra_shot_t : public hunter_ranged_attack_t
{
  double focus_gain;

  cobra_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "cobra_shot", player, player -> find_class_spell( "Cobra Shot" ) )
  {
    parse_options( NULL, options_str );

    direct_power_mod = 0.017; // hardcoded into tooltip


    focus_gain = p() -> dbc.spell( 77443 ) -> effectN( 1 ).base_value();

    // Needs testing
    if ( p() -> set_bonus.tier13_2pc_melee() )
      focus_gain *= 2.0;
  }

  virtual bool usable_moving()
  {
    return p() -> active_aspect == ASPECT_FOX;
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_ranged_attack_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {
      cast_td( s -> target ) -> dots.serpent_sting -> extend_duration( 2 );

      double focus = focus_gain;
      p() -> resource_gain( RESOURCE_FOCUS, focus, p() -> gains.cobra_shot );
    }
  }
};

// Explosive Shot ===========================================================

struct explosive_shot_t : public hunter_ranged_attack_t
{
  int lock_and_load;
  int non_consecutive;

  double base_td_min; double base_td_max;

  explosive_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "explosive_shot", player, player -> find_class_spell( "Explosive Shot" ) ),
    lock_and_load( 0 ), non_consecutive( 0 ),
    base_td_min( 0 ), base_td_max( 0 )
  {
    option_t options[] =
    {
      { "non_consecutive", OPT_BOOL, &non_consecutive },
      { NULL, OPT_UNKNOWN, NULL}
    };

    parse_options( options, options_str );

    may_block = false;
    may_crit  = false;

    tick_power_mod = 0.273; // hardcoded into tooltip
    tick_zero = true;

    base_td_min = player -> dbc.effect_min( data().effectN( 1 ).id(), player -> level );
    base_td_max = player -> dbc.effect_max( data().effectN( 1 ).id(), player -> level );
    if ( sim -> debug ) sim -> output( "Player %s setting Explosive Shot base_td_min=%.2f base_td_max=%.2f", p() -> name(), base_td_min, base_td_max );
  }

  virtual double cost()
  {
    if ( p() -> buffs.lock_and_load -> check() )
      return 0;

    return hunter_ranged_attack_t::cost();
  }

  virtual bool ready()
  {
    if ( cooldown -> remains() == timespan_t::zero() && ! p() -> resource_available( RESOURCE_FOCUS, cost() ) )
    {
      if ( sim -> log ) sim -> output( "Player %s was focus starved when Explosive Shot was ready.", p() -> name() );
      p() -> procs.explosive_shot_focus_starved -> occur();
    }

    if ( non_consecutive && p() -> last_foreground_action )
    {
      if ( p() -> last_foreground_action -> name_str == "explosive_shot" )
        return false;
    }

    return hunter_ranged_attack_t::ready();
  }

  virtual void update_ready()
  {
    cooldown -> duration = ( p() -> buffs.lock_and_load -> check() ? timespan_t::zero() : data().cooldown() );

    hunter_ranged_attack_t::update_ready();
  }

  virtual void execute()
  {
    base_td = sim -> averaged_range( base_td_min, base_td_max );
    hunter_ranged_attack_t::execute();

    p() -> buffs.lock_and_load -> up();
    p() -> buffs.lock_and_load -> decrement();
  }
};

// Kill Shot ================================================================

struct kill_shot_t : public hunter_ranged_attack_t
{
  kill_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "kill_shot", player, player -> find_class_spell( "Kill Shot" ) )
  {
    parse_options( NULL, options_str );

    base_dd_min *= weapon_multiplier; // Kill Shot's weapon multiplier applies to the base damage as well
    base_dd_max *= weapon_multiplier;
    direct_power_mod = 0.45 * weapon_multiplier; // and the coefficient too

    normalize_weapon_speed = true;
  }

  virtual bool ready()
  {
    if ( target -> health_percentage() > 20 )
      return false;

    return hunter_ranged_attack_t::ready();
  }
};

// Scatter Shot Attack ======================================================

struct scatter_shot_t : public hunter_ranged_attack_t
{
  scatter_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "scatter_shot", player, player -> find_class_spell( "Scatter Shot" ) )
  {
    parse_options( NULL, options_str );

    normalize_weapon_speed = true;
  }
};

// Serpent Sting Attack =====================================================

struct serpent_sting_burst_t : public hunter_ranged_attack_t
{
  serpent_sting_burst_t( hunter_t* player ) :
    hunter_ranged_attack_t( "serpent_sting_burst", player, spell_data_t::nil() )
  {
    school = SCHOOL_NATURE;
    proc       = true;
    background = true;
  }
};

struct serpent_sting_t : public hunter_ranged_attack_t
{
  serpent_sting_burst_t* serpent_sting_burst;

  serpent_sting_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "serpent_sting", player, player -> find_class_spell( "Serpent Sting" ) )
  {
    parse_options( NULL, options_str );

    const spell_data_t* scaling_data = data().effectN( 1 ).trigger(); // id 118253
    parse_effect_data( scaling_data -> effectN( 1 ) );
    tick_power_mod = scaling_data -> extra_coeff();

    may_block = false;
    may_crit  = false;

    serpent_sting_burst = new serpent_sting_burst_t( p() );
    add_child( serpent_sting_burst );
  }

  virtual void execute()
  {
    hunter_ranged_attack_t::execute();

    if ( serpent_sting_burst && p() -> specs.improved_serpent_sting -> ok() )
    {
      double t = ( p() -> specs.improved_serpent_sting -> effectN( 1 ).percent() ) *
                 ( ceil( base_td ) * hasted_num_ticks( player_haste ) + total_power() * 0.4 );

      serpent_sting_burst -> base_dd_min = t;
      serpent_sting_burst -> base_dd_max = t;
      serpent_sting_burst -> execute();
    }
  }

  virtual void tick( dot_t* d )
  {
    hunter_ranged_attack_t::tick( d );

    if ( p() -> specs.viper_venom -> ok() && p() -> cooldowns.viper_venom -> remains() == timespan_t::zero() )
    {
      double focus_gain = p() -> specs.viper_venom -> effectN( 1 ).trigger() -> effectN( 1 ).base_value();
      p() -> resource_gain( RESOURCE_FOCUS, focus_gain, p() -> gains.viper_venom );
      p() -> cooldowns.viper_venom -> start();
    }
  }
};

struct serpent_sting_spread_t : public serpent_sting_t
{
  serpent_sting_spread_t( hunter_t* player, const std::string& options_str ) :
    serpent_sting_t( player, options_str )
  {
    num_ticks = 1 + p()-> specs.serpent_spread -> ok();
    travel_speed = 0.0;
    background = true;
    aoe = -1;
    serpent_sting_burst -> aoe = -1;
  }

  virtual void execute()
  {
    hunter_ranged_attack_t::execute();

    if ( serpent_sting_burst && p() -> specs.improved_serpent_sting -> ok() )
    {
      double t = ( p() -> specs.improved_serpent_sting -> effectN( 1 ).percent() ) *
                 ( ceil( base_td ) * 5 + total_power() * 0.4 );

      serpent_sting_burst -> base_dd_min = t;
      serpent_sting_burst -> base_dd_max = t;
      serpent_sting_burst -> execute();
    }
  }
};

// Multi Shot Attack ========================================================

struct multi_shot_t : public hunter_ranged_attack_t
{
  serpent_sting_spread_t* spread_sting;

  multi_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "multi_shot", player, player -> find_class_spell( "Multi-Shot" ) ),
    spread_sting( 0 )
  {
    assert( p() -> main_hand_weapon.type != WEAPON_NONE );
    parse_options( NULL, options_str );

    aoe = -1;

    normalize_weapon_speed = true;
    if ( p() -> specs.serpent_spread -> ok() )
      spread_sting = new serpent_sting_spread_t( player, options_str );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_ranged_attack_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {
      if ( spread_sting )
        spread_sting -> execute();
      if ( s -> result == RESULT_CRIT && p() -> specs.bombardment -> ok() )
        p() -> buffs.bombardment -> trigger();

      // TODO determine multishot adds in execute.
      /*for ( int i=0; i < q -> adds_nearby; i++ ) {
        // Calculate a result for each nearby add to determine whether to proc
        // bombardment
        int delta_level = q -> level - p() -> level;
        if ( sim -> real() <= crit_chance( delta_level ) )
          crit_occurred++;
      }*/
    }
  }

  virtual double cost()
  {
    if ( p() -> buffs.bombardment -> check() )
      return base_costs[ current_resource() ] * ( 1 + p() -> buffs.bombardment -> data().effectN( 1 ).percent() );

    return hunter_ranged_attack_t::cost();
  }
};

// Silencing Shot Attack ====================================================

struct silencing_shot_t : public hunter_ranged_attack_t
{
  silencing_shot_t( hunter_t* player, const std::string& /* options_str */ ) :
    hunter_ranged_attack_t( "silencing_shot", player, player -> find_class_spell( "Silencing Shot" ) )
  {
    weapon_multiplier = 0.0;

    may_miss = may_glance = may_block = may_dodge = may_parry = may_crit = false;

    normalize_weapon_speed = true;
  }
};

// Steady Shot Attack =======================================================

struct steady_shot_t : public hunter_ranged_attack_t
{
  double focus_gain;

  steady_shot_t( hunter_t* player, const std::string& options_str ) :
    hunter_ranged_attack_t( "steady_shot", player, player -> find_class_spell( "Steady Shot" ) )
  {
    parse_options( NULL, options_str );

    direct_power_mod = 0.021; // hardcoded into tooltip

    focus_gain = p() -> dbc.spell( 77443 ) -> effectN( 1 ).base_value();

    // Needs testing
    if ( p() -> set_bonus.tier13_2pc_melee() )
      focus_gain *= 2.0;
  }

  virtual void trigger_steady_focus()
  {
    p() -> buffs.pre_steady_focus -> trigger( 1 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_ranged_attack_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {

      p() -> resource_gain( RESOURCE_FOCUS, focus_gain, p() -> gains.steady_shot );

      if ( ! p() -> buffs.master_marksman_fire -> check() && p() -> buffs.master_marksman -> trigger() )
      {
        if ( p() -> buffs.master_marksman -> stack() == 5 )
        {
          p() -> buffs.master_marksman_fire -> trigger();
          p() -> buffs.master_marksman -> expire();
        }
      }

      if ( s -> result == RESULT_CRIT )
        trigger_piercing_shots( this, s -> target, s -> result_amount );
    }
  }

  virtual bool usable_moving()
  {
    return p() -> active_aspect == ASPECT_FOX;
  }

  virtual double composite_target_crit( player_t* t )
  {
    double cc = hunter_ranged_attack_t::composite_target_crit( t );

    if ( p() -> specs.careful_aim -> ok() && t -> health_percentage() > p() -> specs.careful_aim -> effectN( 2 ).base_value() )
    {
      cc += p() -> specs.careful_aim -> effectN( 1 ).percent();
    }

    return cc;
  }
};

// Wild Quiver ==============================================================

struct wild_quiver_shot_t : public ranged_t
{
  wild_quiver_shot_t( hunter_t* p ) : ranged_t( p, "wild_quiver_shot" )
  {
    repeating   = false;
    proc = true;
    normalize_weapon_speed=true;
  }
};

} // end attacks
// ==========================================================================
// Hunter Spells
// ==========================================================================

namespace spells {
struct hunter_spell_t : public hunter_action_t<spell_t>
{
public:
  hunter_spell_t( const std::string& n, hunter_t* player,
                  const spell_data_t* s = spell_data_t::nil() ) :
    base_t( n, player, s )
  {
  }

  virtual timespan_t gcd()
  {
    if ( ! harmful && ! player -> in_combat )
      return timespan_t::zero();

    // Hunter gcd unaffected by haste
    return trigger_gcd;
  }
};


// A Murder of Crows ==============================================================

//in your case, instead of felstorm_t and felstorm_tick_t you'd have moc_t, moc_tick_t, and crow_tick_t or something
//the tick_action?  I was assumign I'd have one dot which is the murder spell. it's tick_action would spawn a crow dot.
//and moc_t's tick_action would be an instance of moc_tick_t, and moc_tick_t's tick action would be an instance of crow_tick_t
//and crow_tick_t would be the actual damage, so it'd have SCHOOL_PHYSICAL etc
//and a base damage and an attack power coefficient and whatnot

struct moc_crow_t : public pet_t
{
  struct peck_t : public melee_attack_t
  {
    peck_t( moc_crow_t* player ) :
      melee_attack_t( "crow_peck", player )
    {
      if ( player -> o() -> moc_crows[ 0 ] )
        stats = player -> o() -> moc_crows[ 0 ] -> get_stats( "crow_peck" );

      background  = true;

      weapon = &( player -> main_hand_weapon );
      base_execute_time = weapon -> swing_time;
      base_dd_min = base_dd_max = 1;
      school = SCHOOL_PHYSICAL;

      trigger_gcd = timespan_t::zero();

      background = true;
      repeating  = true;
      special    = false;
      may_glance = true;
      may_crit   = true;
    }
  };

  hunter_t* o() { return static_cast< hunter_t* >( owner ); }

  moc_crow_t( sim_t* sim, hunter_t* owner ) :
    pet_t( sim, owner, "moc_crow", true /*GUARDIAN*/ )
  {
    // FIX ME
    owner_coeff.ap_from_ap = 1.00;
  }

  virtual void init_base()
  {
    pet_t::init_base();

    // FIXME numbers are from treant. correct them.
    resources.base[ RESOURCE_HEALTH ] = 9999; // Level 85 value
    resources.base[ RESOURCE_MANA   ] = 0;

    stamina_per_owner = 0;

    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 580;
    main_hand_weapon.max_dmg    = 580;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = timespan_t::from_seconds( 2 );

    main_hand_attack = new peck_t( this );
  }

  virtual void summon( timespan_t duration=timespan_t::zero() )
  {
    pet_t::summon( duration );
    // Crows cast on the target will instantly perform a melee
    main_hand_attack -> execute();
  }
};

struct moc_t : public hunter_spell_t
{
  int crows_summoned;

  moc_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "a_murder_of_crows", player, player -> find_talent_spell( "A Murder of Crows" ) )
  {
    crows_summoned = 0;

    hasted_ticks = false;
    may_crit = true;

    base_tick_time = timespan_t::from_seconds( 2.0 );
    // The eighth summons comes from tick_zero.  Unfortunately tick number
    // does not inlude tick_zero, so we need our own counter.
    num_ticks = 7;
    tick_zero = true;

    dynamic_tick_action = true;
  }

  virtual void execute()
  {
    crows_summoned = 0;
    hunter_spell_t::execute();
    // FIXME I'd rather have this get reset on expiration so that it could potentially stack
  }

  virtual void tick( dot_t* d )
  {
    hunter_spell_t::tick( d );
    if ( crows_summoned < CROW_LIMIT )
    {
      p() -> moc_crows[ crows_summoned ] -> summon( timespan_t::from_seconds( 16 ) );
      crows_summoned++;
    }
  }
};

// Aspect of the Hawk =======================================================

struct aspect_of_the_hawk_t : public hunter_spell_t
{
  aspect_of_the_hawk_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "aspect_of_the_hawk", player, player -> find_class_spell( "Aspect of the Hawk" ) )
  {
    parse_options( NULL, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    hunter_spell_t::execute();

    if ( !p() -> active_aspect )
    {
      p() -> active_aspect = ASPECT_HAWK;
      double value = p() -> dbc.effect_average( data().effect_id( 1 ), p() -> level );
      p() -> buffs.aspect_of_the_hawk -> trigger( 1, value );
    }
    else if ( p() -> active_aspect == ASPECT_HAWK )
    {
      p() -> active_aspect = ASPECT_NONE;
      p() -> buffs.aspect_of_the_hawk -> expire();
    }
    else if ( p() -> active_aspect == ASPECT_FOX )
    {
      p() -> active_aspect = ASPECT_HAWK;
      double value = p() -> dbc.effect_average( data().effect_id( 1 ), p() -> level );
      p() -> buffs.aspect_of_the_hawk -> trigger( 1, value );
    }

  }
  virtual bool ready()
  {
    if ( p() -> active_aspect == ASPECT_HAWK )
      return false;

    return hunter_spell_t::ready();
  }
};

// Aspect of the Fox ========================================================

struct aspect_of_the_fox_t : public hunter_spell_t
{
  aspect_of_the_fox_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "aspect_of_the_fox", player, player -> find_class_spell( "Aspect of the Fox" ) )
  {
    parse_options( NULL, options_str );
    harmful = false;
  }

  virtual void execute()
  {
    hunter_spell_t::execute();

    if ( !p() -> active_aspect )
    {
      p() -> active_aspect = ASPECT_FOX;
    }
    else if ( p() -> active_aspect == ASPECT_FOX )
    {
      p() -> active_aspect = ASPECT_NONE;
    }
    else if ( p() -> active_aspect == ASPECT_HAWK )
    {
      p() -> buffs.aspect_of_the_hawk -> expire();
      p() -> active_aspect = ASPECT_FOX;
    }

  }
  virtual bool ready()
  {
    if ( p() -> active_aspect == ASPECT_FOX )
      return false;

    return hunter_spell_t::ready();
  }
};

// Bestial Wrath ============================================================

struct bestial_wrath_t : public hunter_spell_t
{
  bestial_wrath_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "bestial_wrath", player, player -> find_class_spell( "Bestial Wrath" ) )
  {
    parse_options( NULL, options_str );
    harmful = false;
  }

  virtual void execute()
  {
    p() -> buffs.beast_within  -> trigger();
    p() -> active_pet -> buffs.bestial_wrath -> trigger();

    hunter_spell_t::execute();
  }

  virtual bool ready()
  {
    if ( ! p() -> active_pet )
      return false;

    return hunter_spell_t::ready();
  }
};

// Fervor ===================================================================

struct fervor_t : public hunter_spell_t
{
  fervor_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "fervor", player, player -> talents.fervor )
  {
    parse_options( NULL, options_str );

    harmful = false;

    trigger_gcd = timespan_t::zero();
  }

  virtual void execute()
  {
    if ( p() -> active_pet )
      p() -> active_pet -> resource_gain( RESOURCE_FOCUS, data().effectN( 1 ).base_value(), p() -> active_pet -> gains.fervor );

    p() -> resource_gain( RESOURCE_FOCUS, data().effectN( 1 ).base_value(), p() -> gains.fervor );

    hunter_spell_t::execute();
  }
};

// Focus Fire ===============================================================

struct focus_fire_t : public hunter_spell_t
{
  int five_stacks;
  focus_fire_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "focus_fire", player, player -> find_spell( 82692 ) )
  {
    option_t options[] =
    {
      { "five_stacks", OPT_BOOL, &five_stacks },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    harmful = false;
  }

  virtual void execute()
  {
    double value = p() -> active_pet -> buffs.frenzy -> stack() * p() -> specs.focus_fire -> effectN( 3 ).percent();
    p() -> buffs.focus_fire -> trigger( 1, value );

    double gain = p() -> specs.focus_fire -> effectN( 2 ).resource( RESOURCE_FOCUS );
    p() -> active_pet -> resource_gain( RESOURCE_FOCUS, gain, p() -> active_pet -> gains.focus_fire );

    hunter_spell_t::execute();

    p() -> active_pet -> buffs.frenzy -> expire();
  }

  virtual bool ready()
  {
    if ( ! p() -> active_pet )
      return false;

    if ( ! p() -> active_pet -> buffs.frenzy -> check() )
      return false;

    if ( five_stacks && p() -> active_pet -> buffs.frenzy -> check() < 5 )
      return false;

    return hunter_spell_t::ready();
  }
};

// Hunter's Mark ============================================================

struct hunters_mark_t : public hunter_spell_t
{
  hunters_mark_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "hunters_mark", player, player -> find_class_spell( "Hunter's Mark" ) )
  {
    parse_options( NULL, options_str );

    harmful = false;
    background = ( sim -> overrides.ranged_vulnerability != 0 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_spell_t::impact_s( s );

    if ( ! sim -> overrides.ranged_vulnerability )
      s -> target -> debuffs.ranged_vulnerability -> trigger();
  }
};

// Kill Command =============================================================

struct kill_command_t : public hunter_spell_t
{
  kill_command_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "kill_command", player, player -> find_class_spell( "Kill Command" ) )
  {
    parse_options( NULL, options_str );

    base_spell_power_multiplier    = 0.0;
    base_attack_power_multiplier   = 1.0;
    // FIXME
    //base_costs[ current_resource() ] += p() -> glyphs.kill_command -> mod_additive( P_RESOURCE_COST );

    harmful = false;

    for ( size_t i = 0, pets = p() -> pet_list.size(); i < pets; ++i )
    {
      pet_t* pet = p() -> pet_list[ i ];
      stats -> children.push_back( pet -> get_stats( "kill_command" ) );
    }
  }

  virtual void execute()
  {
    hunter_spell_t::execute();

    if ( p() -> active_pet )
    {
      p() -> active_pet -> kill_command -> base_dd_adder = 0.516 * execute_state -> attack_power; // hardcoded into tooltip
      p() -> active_pet -> kill_command -> execute();
    }
  }

  virtual bool ready()
  {
    if ( ! p() -> active_pet )
      return false;

    return hunter_spell_t::ready();
  }
};


// Rapid Fire ===============================================================

struct rapid_fire_t : public hunter_spell_t
{
  rapid_fire_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "rapid_fire", player, player -> find_class_spell( "Rapid Fire" ) )
  {
    parse_options( NULL, options_str );

//    cooldown -> duration += p() -> talents.posthaste -> effectN( 1 ).time_value();

    harmful = false;
  }

  virtual void execute()
  {
    double value = data().effectN( 1 ).percent();
    p() -> buffs.rapid_fire -> trigger( 1, value );

    hunter_spell_t::execute();
  }

  virtual bool ready()
  {
    if ( p() -> buffs.rapid_fire -> check() )
      return false;

    return hunter_spell_t::ready();
  }
};

// Readiness ================================================================

struct readiness_t : public hunter_spell_t
{
  int wait_for_rf;
  std::vector<cooldown_t*> cooldown_list;

  readiness_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "readiness", player, player -> talents.readiness ),
    wait_for_rf( false )
  {
    option_t options[] =
    {
      // Only perform Readiness while Rapid Fire is up, allows the sequence
      // Rapid Fire, Readiness, Rapid Fire, for better RF uptime
      { "wait_for_rapid_fire", OPT_BOOL, &wait_for_rf },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    harmful = false;

    cooldown_list.push_back( p() -> get_cooldown( "traps"            ) );
    cooldown_list.push_back( p() -> get_cooldown( "chimera_shot"     ) );
    cooldown_list.push_back( p() -> get_cooldown( "kill_shot"        ) );
    cooldown_list.push_back( p() -> get_cooldown( "scatter_shot"     ) );
    cooldown_list.push_back( p() -> get_cooldown( "silencing_shot"   ) );
    cooldown_list.push_back( p() -> get_cooldown( "kill_command"     ) );
    cooldown_list.push_back( p() -> get_cooldown( "rapid_fire"       ) );
    cooldown_list.push_back( p() -> get_cooldown( "bestial_wrath"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "concussive_shot"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "dire_beast"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "powershot"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "barrage"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "lynx_rush"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "a_murder_of_crows"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "glaive_toss"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "deterrence"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "distracting_shot"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "freezing_trap"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "frost_trap"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "explosive_trap"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "explosive_shot"    ) );
    cooldown_list.push_back( p() -> get_cooldown( "lock_and_load"    ) ); //????
  }

  virtual void execute()
  {
    hunter_spell_t::execute();

    for ( unsigned int i=0; i < cooldown_list.size(); i++ )
    {
      cooldown_list[ i ] -> reset();
    }
  }

  virtual bool ready()
  {

    if ( wait_for_rf && ! p() -> buffs.rapid_fire -> check() )
      return false;

    return hunter_spell_t::ready();
  }
};

// Summon Pet ===============================================================

struct summon_pet_t : public hunter_spell_t
{
  std::string pet_name;
  pet_t* pet;

  summon_pet_t( hunter_t* player, const std::string& options_str ) :
    hunter_spell_t( "summon_pet", player, spell_data_t::nil() ),
    pet( 0 )
  {

    harmful = false;

    pet_name = ( options_str.size() > 0 ) ? options_str : p() -> summon_pet_str;
    pet = p() -> find_pet( pet_name );
    if ( ! pet )
    {
      sim -> errorf( "Player %s unable to find pet %s for summons.\n", p() -> name(), pet_name.c_str() );
      sim -> cancel();
    }
  }

  virtual void execute()
  {
    hunter_spell_t::execute();

    pet -> summon();

    p() -> main_hand_attack -> cancel();
  }

  virtual bool ready()
  {
    if ( p() -> active_pet == pet )
      return false;

    return hunter_spell_t::ready();
  }
};

// Trueshot Aura ============================================================

struct trueshot_aura_t : public hunter_spell_t
{
  trueshot_aura_t( hunter_t* player, const std::string& /* options_str */ ) :
    hunter_spell_t( "trueshot_aura", player, player -> find_class_spell( "Trueshot Aura" ) )
  {
    trigger_gcd = timespan_t::zero();
    harmful = false;
    background = ( sim -> overrides.attack_power_multiplier != 0 && sim -> overrides.critical_strike != 0 );
  }

  virtual void execute()
  {
    hunter_spell_t::execute();

    // FIXME do we still need to trigger the aura?  e.g., to manage stacking

    if ( ! sim -> overrides.attack_power_multiplier )
      sim -> auras.attack_power_multiplier -> trigger();

    if ( ! sim -> overrides.critical_strike )
      sim -> auras.critical_strike -> trigger();
  }
};

} // spells

namespace pet_actions {

// Template for common hunter pet action code. See priest_action_t.
template <class Base>
struct hunter_pet_action_t : public Base
{
  typedef Base ab;
  typedef hunter_pet_action_t base_t;

  bool special_ability;

  hunter_pet_action_t( const std::string& n, hunter_pet_t* player,
                       const spell_data_t* s = spell_data_t::nil() ) :
    ab( n, player, s ),
    special_ability( false )
  {
    ab::stateless = true;

    if ( ab::data().rank_str() && !strcmp( ab::data().rank_str(), "Special Ability" ) )
      special_ability = true;
  }

  hunter_pet_t* p() const
  { return static_cast<hunter_pet_t*>( ab::player ); }

  hunter_t* o() const
  { return static_cast<hunter_t*>( p() -> cast_owner() ); }

  hunter_pet_td_t* cast_td( player_t* t = 0 )
  { return p() -> get_target_data( t ? t : ab::target ); }

  void apply_exotic_beast_cd()
  {
    ab::cooldown -> duration *= 1.0 + o() -> specs.exotic_beasts -> effectN( 2 ).percent();
  }
};

// ==========================================================================
// Hunter Pet Attacks
// ==========================================================================

struct hunter_pet_attack_t : public hunter_pet_action_t<attack_t>
{
  hunter_pet_attack_t( const std::string& n, hunter_pet_t* player,
                       const spell_data_t* s = spell_data_t::nil() ) :
    base_t( n, player, s )
  {
    may_crit = true;
  }

  virtual double swing_haste()
  {
    double h = base_t::swing_haste();

    h *= 1.0 / ( 1.0 + p() -> cast_owner() -> specs.frenzy -> effectN( 1 ).percent() * p() -> buffs.frenzy -> stack() );

    return h;
  }

  virtual void impact_s( action_state_t* s )
  {
    base_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {
      if ( p() -> buffs.rabid -> up() )
      {
        p() -> buffs.rabid_power_stack -> trigger();
      }
    }
  }
};

// Pet Melee ================================================================

struct pet_melee_t : public hunter_pet_attack_t
{
  pet_melee_t( hunter_pet_t* player ) :
    hunter_pet_attack_t( "melee", player )
  {
    special = false;
    weapon = &( player -> main_hand_weapon );
    base_execute_time = weapon -> swing_time;
    background        = true;
    repeating         = true;
    school = SCHOOL_PHYSICAL;
    stats -> school = school;
  }
};

// Pet Auto Attack ==========================================================

struct pet_auto_attack_t : public hunter_pet_attack_t
{
  pet_auto_attack_t( hunter_pet_t* player, const std::string& /* options_str */ ) :
    hunter_pet_attack_t( "auto_attack", player, 0 )
  {
    p() -> main_hand_attack = new pet_melee_t( player );
    trigger_gcd = timespan_t::zero();
    school = SCHOOL_PHYSICAL;
    stats -> school = school;
  }

  virtual void execute()
  {
    p() -> main_hand_attack -> schedule_execute();
  }

  virtual bool ready()
  {
    return( p() -> main_hand_attack -> execute_event == 0 ); // not swinging
  }
};

// Pet Claw =================================================================

struct basic_attack_t : public hunter_pet_attack_t
{
  rng_t* rng_invigoration;
  double chance_invigoration;
  double gain_invigoration;

  basic_attack_t( hunter_pet_t* p, const std::string& name, const std::string& options_str ) :
    hunter_pet_attack_t( name, p, p -> find_pet_spell( name ) )
  {
    parse_options( NULL, options_str );
    direct_power_mod = 0.2; // hardcoded into tooltip
    base_multiplier *= 1.0 + p -> talents.spiked_collar -> effectN( 1 ).percent();
    rng_invigoration = player -> get_rng( "invigoration " );
    chance_invigoration = p -> find_spell( 53397 ) -> proc_chance();
    gain_invigoration = p -> find_spell( 53398 ) -> effectN( 1 ).resource( RESOURCE_FOCUS );
  }

  virtual void execute()
  {
    hunter_pet_attack_t::execute();

    o() -> buffs.cobra_strikes -> decrement();
    if ( o() -> specs.frenzy -> ok() )
      p() -> buffs.frenzy -> trigger( 1 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_pet_attack_t::impact_s( s );

    if ( result_is_hit( s -> result ) )
    {
      if ( o() -> specs.invigoration -> ok() && rng_invigoration -> roll( chance_invigoration ) )
      {
        o() -> resource_gain( RESOURCE_FOCUS, gain_invigoration, o() -> gains.invigoration );

        o() -> procs.invigoration -> occur();
      }
    }
  }

  virtual double composite_crit()
  {
    double cc = hunter_pet_attack_t::composite_crit();

    if ( o() -> buffs.cobra_strikes -> up() )
      cc += o() -> buffs.cobra_strikes -> data().effectN( 1 ).percent();

    return cc;
  }

  virtual double action_multiplier()
  {
    double am = ab::action_multiplier();

    if ( p() -> resources.current[ RESOURCE_FOCUS ] > 50 )
    {
      p() -> benefits.wild_hunt -> update( true );
      am *= 1.0 + p() -> specs.wild_hunt -> effectN( 2 ).percent();
    }
    else
      p() -> benefits.wild_hunt -> update( false );

    return am;
  }

  virtual double cost()
  {
    double c = ab::cost();

    if ( p() -> resources.current[ RESOURCE_FOCUS ] > 50 )
      c *= 1.0 + p() -> specs.wild_hunt -> effectN( 1 ).percent();

    return c;
  }

};

// Devilsaur Monstrous Bite =================================================

struct monstrous_bite_t : public hunter_pet_attack_t
{
  monstrous_bite_t( hunter_pet_t* p, const std::string& options_str ) :
    hunter_pet_attack_t( "monstrous_bite", p, p -> find_spell( 54680 ) )
  {
    parse_options( NULL, options_str );
    auto_cast = true;
    apply_exotic_beast_cd();
    school = SCHOOL_PHYSICAL;
    stats -> school = SCHOOL_PHYSICAL;
  }
};

// Kill Command (pet) =======================================================

struct pet_kill_command_t : public hunter_pet_attack_t
{
  pet_kill_command_t( hunter_pet_t* p ) :
    hunter_pet_attack_t( "kill_command", p, p -> find_spell( 83381 ) )
  {
    background = true;
    proc=true;

    base_multiplier *= 1.8; // hardcoded into hunter kill command tooltip
  }
};

// ==========================================================================
// Hunter Pet Spells
// ==========================================================================

struct hunter_pet_spell_t : public hunter_pet_action_t<spell_t>
{
  hunter_pet_spell_t( const std::string& n, hunter_pet_t* player,
                      const spell_data_t* s = spell_data_t::nil() ) :
    base_t( n, player, s )
  {
    apply_exotic_beast_cd();
  }
};

// Rabid ====================================================================

struct rabid_t : public hunter_pet_spell_t
{
  rabid_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "rabid", player, player -> find_spell( 53401 ) )
  {
    parse_options( NULL, options_str );
  }

  virtual void execute()
  {
    p() -> buffs.rabid_power_stack -> expire();
    p() -> buffs.rabid -> trigger();

    hunter_pet_spell_t::execute();
  }
};

// ==========================================================================
// Unique Pet Specials
// ==========================================================================

// Wolf/Devilsaur Furious Howl ==============================================

struct furious_howl_t : public hunter_pet_spell_t
{
  furious_howl_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "furious_howl", player, player -> find_pet_spell( "Furious Howl" ) )
  {

    parse_options( NULL, options_str );

    harmful = false;
    background = ( sim -> overrides.critical_strike != 0 );
  }

  virtual void execute()
  {
    hunter_pet_spell_t::execute();

    if ( ! sim -> overrides.critical_strike )
      sim -> auras.critical_strike -> trigger( 1, -1.0, -1.0, data().duration() );
  }
};

// Cat/Spirit Beast Roar of Courage =========================================

struct roar_of_courage_t : public hunter_pet_spell_t
{
  roar_of_courage_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "roar_of_courage", player, player -> find_pet_spell( "Roar of Courage" ) )
  {

    parse_options( NULL, options_str );

    harmful = false;
    background = ( sim -> overrides.str_agi_int != 0 );
  }

  virtual void execute()
  {
    hunter_pet_spell_t::execute();

    if ( ! sim -> overrides.str_agi_int )
      sim -> auras.str_agi_int -> trigger( 1, -1.0, -1.0, data().duration() );
  }
};

// Silithid Qiraji Fortitude  ===============================================

struct qiraji_fortitude_t : public hunter_pet_spell_t
{
  qiraji_fortitude_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "qiraji_fortitude", player, player -> find_pet_spell( "Qiraji Fortitude" ) )
  {

    parse_options( NULL, options_str );

    harmful = false;
    background = ( sim -> overrides.stamina != 0 );
  }

  virtual void execute()
  {
    hunter_pet_spell_t::execute();

    if ( ! sim -> overrides.stamina )
      sim -> auras.stamina -> trigger();
  }
};

// Wind Serpent Lightning Breath ============================================

struct lightning_breath_t : public hunter_pet_spell_t
{
  lightning_breath_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "lightning_breath", player, player -> find_pet_spell( "Lightning Breath" ) )
  {

    parse_options( 0, options_str );

    auto_cast = true;
    background = ( sim -> overrides.magic_vulnerability != 0 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_pet_spell_t::impact_s( s );

    if ( result_is_hit( s -> result ) && ! sim -> overrides.magic_vulnerability )
      s -> target -> debuffs.magic_vulnerability -> trigger( 1, -1.0, -1.0, data().duration() );
  }
};

// Serpent Corrosive Spit  ==================================================

struct corrosive_spit_t : public hunter_pet_spell_t
{
  corrosive_spit_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "corrosive_spit", player, player -> find_spell( 95466 ) )
  {

    parse_options( 0, options_str );

    auto_cast = true;
    background = ( sim -> overrides.weakened_armor != 0 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_pet_spell_t::impact_s( s );

    if ( result_is_hit( s -> result ) && ! sim -> overrides.weakened_armor )
      s -> target -> debuffs.weakened_armor -> trigger( 1, -1.0, -1.0, data().duration() );
  }
};

// Demoralizing Screech  ====================================================

struct demoralizing_screech_t : public hunter_pet_spell_t
{
  demoralizing_screech_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "demoralizing_screech", player, player -> find_spell( 24423 ) )
  {

    parse_options( 0, options_str );

    auto_cast = true;
    background = ( sim -> overrides.weakened_blows != 0 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_pet_spell_t::impact_s( s );

    // TODO: Is actually an aoe ability
    if ( result_is_hit( s -> result ) && ! sim -> overrides.weakened_blows )
      s -> target -> debuffs.weakened_blows -> trigger( 1, -1.0, -1.0, data().duration() );
  }
};

// Ravager Ravage ===========================================================

struct ravage_t : public hunter_pet_spell_t
{
  ravage_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "ravage", player, player -> find_spell( 50518 ) )
  {
    parse_options( 0, options_str );

    auto_cast = true;
  }
};

// Raptor Tear Armor  =======================================================

struct tear_armor_t : public hunter_pet_spell_t
{
  tear_armor_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "tear_armor", player, player -> find_spell( 50498 ) )
  {
    parse_options( 0, options_str );

    auto_cast = true;

    background = ( sim -> overrides.weakened_armor != 0 );
  }

  virtual void impact_s( action_state_t* s )
  {
    hunter_pet_spell_t::impact_s( s );

    if ( result_is_hit( s -> result ) && ! sim -> overrides.weakened_armor )
      s -> target -> debuffs.weakened_armor -> trigger( 1, -1.0, -1.0, data().duration() );
  }
};

// Hyena Cackling Howl =========================================================

struct cackling_howl_t : public hunter_pet_spell_t
{
  cackling_howl_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "cackling_howl", player, player -> find_spell( 50271 ) )
  {
    parse_options( 0, options_str );

    auto_cast = true;
  }
};

// Chimera Froststorm Breath ================================================

struct froststorm_breath_t : public hunter_pet_spell_t
{
  struct froststorm_breath_tick_t : public hunter_pet_spell_t
  {
    froststorm_breath_tick_t( hunter_pet_t* player ) :
      hunter_pet_spell_t( "froststorm_breath_tick", player, player -> find_spell( 95725 ) )
    {
      direct_power_mod = 0.24; // hardcoded into tooltip, 17/10/2011
      background  = true;
      direct_tick = true;
    }
  };

  froststorm_breath_tick_t* tick_spell;

  froststorm_breath_t( hunter_pet_t* player, const std::string& options_str ) :
    hunter_pet_spell_t( "froststorm_breath", player, player -> find_pet_spell( "Froststorm Breath" ) )
  {
    channeled = true;

    parse_options( NULL, options_str );

    auto_cast = true;

    tick_spell = new froststorm_breath_tick_t( player );

    add_child( tick_spell );
  }

  virtual void init()
  {
    hunter_pet_spell_t::init();

    tick_spell -> stats = stats;
  }

  virtual void tick( dot_t* d )
  {
    tick_spell -> execute();
    stats -> add_tick( d -> time_to_tick );
  }
};

} // pet_actions

struct wild_quiver_trigger_t : public action_callback_t
{
  attack_t* attack;
  rng_t* rng;

  wild_quiver_trigger_t( hunter_t* p, attack_t* a ) :
    action_callback_t( p ), attack( a )
  {
    rng = p -> get_rng( "wild_quiver" );
  }

  virtual void trigger( action_t* a, void* /* call_data */ )
  {
    hunter_t* p = static_cast<hunter_t*>( listener );

    if ( ! a -> weapon )
      return;

    if ( a -> weapon -> slot != SLOT_MAIN_HAND )
      return;

    if ( a -> proc )
      return;

    if ( rng -> roll( p -> composite_mastery() * p -> mastery.wild_quiver -> effectN( 1 ).coeff() / 100.0 ) )
    {
      attack -> execute();
      p -> procs.wild_quiver -> occur();
    }
  }
};

hunter_td_t::hunter_td_t( player_t* target, hunter_t* p ) :
  actor_pair_t( target, p ),
  dots( dots_t() )
{
  dots.serpent_sting = target -> get_dot( "serpent_sting", p );
}

hunter_pet_td_t::hunter_pet_td_t( player_t* target, hunter_pet_t* p ) :
  actor_pair_t( target, p )
{
}
// hunter_pet_t::create_action ==============================================

action_t* hunter_pet_t::create_action( const std::string& name,
                                       const std::string& options_str )
{
  if ( name == "auto_attack"           ) return new      pet_auto_attack_t( this, options_str );
  if ( name == "claw"                  ) return new         basic_attack_t( this, "Claw", options_str );
  if ( name == "bite"                  ) return new         basic_attack_t( this, "Bite", options_str );
  if ( name == "smack"                  ) return new        basic_attack_t( this, "Smack", options_str );
  if ( name == "froststorm_breath"     ) return new    froststorm_breath_t( this, options_str );
  if ( name == "furious_howl"          ) return new         furious_howl_t( this, options_str );
  if ( name == "roar_of_courage"       ) return new      roar_of_courage_t( this, options_str );
  if ( name == "qiraji_fortitude"      ) return new     qiraji_fortitude_t( this, options_str );
  if ( name == "lightning_breath"      ) return new     lightning_breath_t( this, options_str );
  if ( name == "monstrous_bite"        ) return new       monstrous_bite_t( this, options_str );
  if ( name == "rabid"                 ) return new                rabid_t( this, options_str );
  if ( name == "corrosive_spit"        ) return new       corrosive_spit_t( this, options_str );
  if ( name == "demoralizing_screech"  ) return new demoralizing_screech_t( this, options_str );
  if ( name == "ravage"                ) return new               ravage_t( this, options_str );
  if ( name == "tear_armor"            ) return new           tear_armor_t( this, options_str );
  if ( name == "cackling_howl"         ) return new        cackling_howl_t( this, options_str );
  if ( name == "corrosive_spit"        ) return new       corrosive_spit_t( this, options_str );

  return pet_t::create_action( name, options_str );
}

// hunter_t::create_action ==================================================

action_t* hunter_t::create_action( const std::string& name,
                                   const std::string& options_str )
{
  if ( name == "auto_shot"             ) return new              auto_shot_t( this, options_str );
  if ( name == "aimed_shot"            ) return new             aimed_shot_t( this, options_str );
  if ( name == "arcane_shot"           ) return new            arcane_shot_t( this, options_str );
  if ( name == "aspect_of_the_hawk"    ) return new     aspect_of_the_hawk_t( this, options_str );
  if ( name == "aspect_of_the_fox"     ) return new     aspect_of_the_fox_t( this, options_str );
  if ( name == "bestial_wrath"         ) return new          bestial_wrath_t( this, options_str );
  if ( name == "black_arrow"           ) return new            black_arrow_t( this, options_str );
  if ( name == "chimera_shot"          ) return new           chimera_shot_t( this, options_str );
  if ( name == "explosive_shot"        ) return new         explosive_shot_t( this, options_str );
  if ( name == "explosive_trap"        ) return new         explosive_trap_t( this, options_str );
  if ( name == "fervor"                ) return new                 fervor_t( this, options_str );
  if ( name == "focus_fire"            ) return new             focus_fire_t( this, options_str );
  if ( name == "hunters_mark"          ) return new           hunters_mark_t( this, options_str );
  if ( name == "kill_command"          ) return new           kill_command_t( this, options_str );
  if ( name == "kill_shot"             ) return new              kill_shot_t( this, options_str );
  if ( name == "multi_shot"            ) return new             multi_shot_t( this, options_str );
  if ( name == "rapid_fire"            ) return new             rapid_fire_t( this, options_str );
  if ( name == "readiness"             ) return new              readiness_t( this, options_str );
  if ( name == "scatter_shot"          ) return new           scatter_shot_t( this, options_str );
  if ( name == "serpent_sting"         ) return new          serpent_sting_t( this, options_str );
  if ( name == "silencing_shot"        ) return new         silencing_shot_t( this, options_str );
  if ( name == "steady_shot"           ) return new            steady_shot_t( this, options_str );
  if ( name == "summon_pet"            ) return new             summon_pet_t( this, options_str );
  if ( name == "trueshot_aura"         ) return new          trueshot_aura_t( this, options_str );
  if ( name == "cobra_shot"            ) return new             cobra_shot_t( this, options_str );
  if ( name == "a_murder_of_crows"     ) return new             moc_t( this, options_str );

#if 0
  if ( name == "trap_launcher"         ) return new          trap_launcher_t( this, options_str );
  if ( name == "binding_shot"          ) return new           binding_shot_t( this, options_str );
  if ( name == "powershot"             ) return new             power_shot_t( this, options_str );
  if ( name == "glaive_toss"           ) return new            glaive_toss_t( this, options_str );
  if ( name == "aspect_of_the_iron_hawk" ) return new      aspect_of_the_iron_hawk_t( this, options_str );
  if ( name == "a_murder_of_crows"     ) return new      a_murder_of_crows_t( this, options_str );
  if ( name == "lynx_rush"             ) return new              lynx_rush_t( this, options_str );
  if ( name == "dire_beast"            ) return new             dire_beast_t( this, options_str );
  if ( name == "barrage"               ) return new                barrage_t( this, options_str );
  if ( name == "wyvern_sting"          ) return new           wyvern_sting_t( this, options_str );
#endif
  return player_t::create_action( name, options_str );
}

// hunter_t::create_pet =====================================================

pet_t* hunter_t::create_pet( const std::string& pet_name,
                             const std::string& pet_type )
{
  pet_t* p = find_pet( pet_name );

  if ( p )
    return p;

  pet_e type = util::parse_pet_type( pet_type );

  if ( type > PET_NONE && type < PET_HUNTER )
  {
    return new hunter_pet_t( sim, this, pet_name, type );
  }
  else
  {
    sim -> errorf( "Player %s with pet %s has unknown type %s\n", name(), pet_name.c_str(), pet_type.c_str() );
    sim -> cancel();
  }

  return 0;
}

// hunter_t::create_pets ====================================================

void hunter_t::create_pets()
{
  create_pet( "cat",          "cat"          );
  create_pet( "devilsaur",    "devilsaur"    );
  create_pet( "raptor",       "raptor"       );
  create_pet( "wind_serpent", "wind_serpent" );
  create_pet( "wolf",         "wolf"         );

  for ( int i = 0; i < CROW_LIMIT; i++ )
  {
    moc_crows[ i ] = new moc_crow_t( sim, this );
    if ( i > 0 )
      moc_crows[ i ] -> quiet = 1;
  }
}

// hunter_t::init_spells ====================================================

void hunter_t::init_spells()
{
  player_t::init_spells();

  // Talents
  talents.posthaste                         = find_talent_spell( "Posthaste" );
  talents.narrow_escape                     = find_talent_spell( "Narrow Escape" );
  talents.exhilaration                      = find_talent_spell( "Exhilaration" );

  talents.silencing_shot                    = find_talent_spell( "Silencing Shot" );
  talents.wyvern_sting                      = find_talent_spell( "Wyvern Sting" );
  talents.binding_shot                      = find_talent_spell( "Binding Shot" );

  talents.crouching_tiger_hidden_chimera    = find_talent_spell( "Courching Tiger, Hiden Chimera" );
  talents.aspect_of_the_iron_hawk           = find_talent_spell( "Aspect of the Iron Hawk" );
  talents.spirit_bond                       = find_talent_spell( "Improved" );

  talents.fervor                            = find_talent_spell( "Fervor" );
  talents.readiness                         = find_talent_spell( "Readiness" );
  talents.thrill_of_the_hunt                = find_talent_spell( "Thrill of the Hunt" );

  talents.a_murder_of_crows                 = find_talent_spell( "A Murder of Crows" );
  talents.dire_beast                        = find_talent_spell( "Dire Beast" );
  talents.lynx_rush                         = find_talent_spell( "Lynx Rush" );

  talents.glaive_toss                       = find_talent_spell( "Glaive Toss" );
  talents.powershot                         = find_talent_spell( "Powershot" );
  talents.barrage                           = find_talent_spell( "Barrage" );

  // Mastery
  mastery.master_of_beasts     = find_mastery_spell( HUNTER_BEAST_MASTERY );
  mastery.wild_quiver          = find_mastery_spell( HUNTER_MARKSMANSHIP );
  mastery.essence_of_the_viper = find_mastery_spell( HUNTER_SURVIVAL );

  // Major
  glyphs.aimed_shot          = find_glyph_spell( "Glyph of Aimed Shot"     ); // id=119465
  glyphs.endless_wrath       = find_glyph_spell( "Glyph of Endless Wrath"  );
  glyphs.animal_bond         = find_glyph_spell( "Glyph of Animal Bond"    );
  glyphs.black_ice           = find_glyph_spell( "Glyph of Block Ice"  );
  glyphs.camouflage          = find_glyph_spell( "Glyph of Camoflage"  );
  glyphs.chimera_shot        = find_glyph_spell( "Glyph of Chimera Shot"   );
  glyphs.deterrence          = find_glyph_spell( "Glyph of Deterrence"  );
  glyphs.disengage           = find_glyph_spell( "Glyph of Disengage"  );
  glyphs.distracting_shot    = find_glyph_spell( "Glyph of Distracting Shot"  );
  glyphs.explosive_trap      = find_glyph_spell( "Glyph of Explosive Trap"  );
  glyphs.freezing_trap       = find_glyph_spell( "Glyph of Freezing Trap"  );
  glyphs.ice_trap            = find_glyph_spell( "Glyph of Ice Trap"  );
  glyphs.icy_solace          = find_glyph_spell( "Glyph of Icy Solace"  );
  glyphs.marked_for_death    = find_glyph_spell( "Glyph of Marked for Death"  );
  glyphs.masters_call        = find_glyph_spell( "Glyph of Master's Call"  );
  glyphs.mend_pet            = find_glyph_spell( "Glyph of Mend Pet"  );
  glyphs.mending             = find_glyph_spell( "Glyph of Mending"  );
  glyphs.mirrored_blades     = find_glyph_spell( "Glyph of Mirrored Blades"  );
  glyphs.misdirection        = find_glyph_spell( "Glyph of Misdirection"  );
  glyphs.no_escape           = find_glyph_spell( "Glyph of No Escape"  );
  glyphs.pathfinding         = find_glyph_spell( "Glyph of Pathfinding"  );
  glyphs.scatter_shot        = find_glyph_spell( "Glyph of Scatter Shot"  );
  glyphs.scattering          = find_glyph_spell( "Glyph of Scattering"  );
  glyphs.snake_trap          = find_glyph_spell( "Glyph of Snake Trap"  );
  glyphs.tranquilizing_shot  = find_glyph_spell( "Glyph of Tranquilizing Shot"  );

  // Minor
  glyphs.aspects             = find_glyph_spell( "Glyph of Aspects"  );
  glyphs.aspect_of_the_pack  = find_glyph_spell( "Glyph of Aspect of the Pack"  );
  glyphs.direction           = find_glyph_spell( "Glyph of Direction"  );
  glyphs.fetch               = find_glyph_spell( "Glyph of Fetch"  );
  glyphs.fireworks           = find_glyph_spell( "Glyph of Fireworks"  );
  glyphs.lesser_proportion   = find_glyph_spell( "Glyph of Lesser Proportion"  );
  glyphs.marking             = find_glyph_spell( "Glyph of Marking"  );
  glyphs.revive_pet          = find_glyph_spell( "Glyph of Revive Pet"  );
  glyphs.stampede            = find_glyph_spell( "Glyph of Stampede"  );
  glyphs.tame_beast          = find_glyph_spell( "Glyph of Tame Beast"  );
  glyphs.the_cheetah         = find_glyph_spell( "Glyph of the Cheetah"  );

  specs.piercing_shots       = find_specialization_spell( "Piercing Shots" );
  specs.steady_focus         = find_specialization_spell( "Steady Focus" );
  specs.go_for_the_throat    = find_specialization_spell( "Go for the Throat" );
  specs.careful_aim          = find_specialization_spell( "Careful Aim" );
  specs.beast_cleave         = find_specialization_spell( "Beast Cleave" );
  specs.frenzy               = find_specialization_spell( "Frenzy" );
  specs.focus_fire           = find_specialization_spell( "Focus Fire" );
  specs.cobra_strikes        = find_specialization_spell( "Cobra Strikes" );
  specs.the_beast_within     = find_specialization_spell( "The Beast Within" );
  specs.exotic_beasts        = find_specialization_spell( "Exotic Beasts" );
  specs.invigoration         = find_specialization_spell( "Invigoration" );
  specs.careful_aim          = find_specialization_spell( "Careful Aim" );
  specs.improved_serpent_sting = find_specialization_spell( "Improved Serpent Sting" );
  specs.viper_venom          = find_specialization_spell( "Viper Venom" );
  specs.bombardment          = find_specialization_spell( "Bombardment" );
  specs.rapid_recuperation   = find_specialization_spell( "Rapid Recuperation" );
  specs.master_marksman      = find_specialization_spell( "Master Marksman" );
  specs.serpent_spread       = find_specialization_spell( "Serpent Spread" );
  specs.trap_mastery         = find_specialization_spell( "Trap Mastery" );


  if ( specs.piercing_shots -> ok() )
    active_piercing_shots = new piercing_shots_t( this );

  static const uint32_t set_bonuses[N_TIER][N_TIER_BONUS] =
  {
    //  C2P    C4P     M2P     M4P    T2P    T4P     H2P    H4P
    {     0,     0, 105732, 105921,     0,     0,     0,     0 }, // Tier13
    {     0,     0,      0,      0,     0,     0,     0,     0 },
  };

  sets = new set_bonus_array_t( this, set_bonuses );
}

// hunter_t::init_spells ====================================================

void hunter_pet_t::init_spells()
{
  pet_t::init_spells();

  kill_command = new pet_kill_command_t( this );

  specs.wild_hunt = find_spell( 62762 );
}

// hunter_t::init_base ======================================================

void hunter_t::init_base()
{
  player_t::init_base();

  base.attack_power = level * 2;

  initial.attack_power_per_strength = 0.0; // Prevents scaling from strength. Will need to separate melee and ranged AP if this is needed in the future.
  initial.attack_power_per_agility  = 2.0;

  // FIXME!
  base_focus_regen_per_second = 4;

  resources.base[ RESOURCE_FOCUS ] = 100 /*+ talents.kindred_spirits -> effectN( 1 ).resource( RESOURCE_FOCUS )*/;

  diminished_kfactor    = 0.009880;
  diminished_dodge_capi = 0.006870;
  diminished_parry_capi = 0.006870;
}

// hunter_t::init_buffs =====================================================

void hunter_t::init_buffs()
{
  player_t::init_buffs();

  // buff_t( player, name, max_stack, duration, chance=-1, cd=-1, quiet=false, reverse=false, activated=true )
  // buff_t( player, id, name, chance=-1, cd=-1, quiet=false, reverse=false, activated=true )
  // buff_t( player, name, spellname, chance=-1, cd=-1, quiet=false, reverse=false, activated=true )

  buffs.aspect_of_the_hawk          = buff_creator_t( this, 13165, "aspect_of_the_hawk" );
  buffs.beast_within                = buff_creator_t( this, 34471, "beast_within" ).chance( specs.the_beast_within -> ok() );
  buffs.bombardment                 = buff_creator_t( this, specs.bombardment -> ok() ? 35104 : 0, "bombardment" );
  buffs.cobra_strikes               = buff_creator_t( this, 53257, "cobra_strikes" ).chance( specs.cobra_strikes -> proc_chance() );
  buffs.focus_fire                  = buff_creator_t( this, 82692, "focus_fire" );
  buffs.steady_focus                = buff_creator_t( this, 53220, "steady_focus" ).chance( specs.steady_focus -> ok() );
  buffs.lock_and_load               = buff_creator_t( this, 56453, "lock_and_load" ).chance( find_spell( 56343 ) -> effectN( 1 ).percent() );
//  if ( bugs ) buffs.lock_and_load -> cooldown -> duration = timespan_t::from_seconds( 10.0 ); // http://elitistjerks.com/f74/t65904-hunter_dps_analyzer/p31/#post2050744
  buffs.master_marksman             = buff_creator_t( this, 82925, "master_marksman" ).chance( specs.master_marksman -> proc_chance() );
  buffs.master_marksman_fire        = buff_creator_t( this, 82926, "master_marksman_fire" );

  buffs.rapid_fire                  = buff_creator_t( this, 3045, "rapid_fire" );
  buffs.rapid_fire -> cooldown -> duration = timespan_t::zero();
  buffs.pre_steady_focus            = buff_creator_t( this, "pre_steady_focus" ).max_stack( 2 ).quiet( true );

  buffs.tier13_4pc                  = buff_creator_t( this, 105919, "tier13_4pc" ).chance( sets -> set( SET_T13_4PC_MELEE ) -> proc_chance() ).duration( timespan_t::from_seconds( tier13_4pc_cooldown ) );

}

// hunter_t::init_values ====================================================

void hunter_t::init_values()
{
  player_t::init_values();

  if ( set_bonus.pvp_2pc_melee() )
    initial.attribute[ ATTR_AGILITY ]   += 70;

  if ( set_bonus.pvp_4pc_melee() )
    initial.attribute[ ATTR_AGILITY ]   += 90;

  cooldowns.viper_venom -> duration = timespan_t::from_seconds( 3.0 ); // hardcoded into tooltip
}

// hunter_t::init_gains =====================================================

void hunter_t::init_gains()
{
  player_t::init_gains();

  gains.invigoration         = get_gain( "invigoration"         );
  gains.fervor               = get_gain( "fervor"               );
  gains.focus_fire           = get_gain( "focus_fire"           );
  gains.rapid_recuperation   = get_gain( "rapid_recuperation"   );
  gains.thrill_of_the_hunt   = get_gain( "thrill_of_the_hunt"   );
  gains.steady_shot          = get_gain( "steady_shot"          );
  gains.cobra_shot           = get_gain( "cobra_shot"           );
  gains.viper_venom          = get_gain( "viper_venom"           );
}

// hunter_t::init_position ==================================================

void hunter_t::init_position()
{
  player_t::init_position();

  if ( position == POSITION_FRONT )
  {
    position = POSITION_RANGED_FRONT;
    position_str = util::position_type_string( position );
  }
  else if ( position == POSITION_BACK )
  {
    position = POSITION_RANGED_BACK;
    position_str = util::position_type_string( position );
  }
}

// hunter_t::init_procs =====================================================

void hunter_t::init_procs()
{
  player_t::init_procs();

  procs.invigoration                 = get_proc( "invigoration"                 );
  procs.thrill_of_the_hunt           = get_proc( "thrill_of_the_hunt"           );
  procs.wild_quiver                  = get_proc( "wild_quiver"                  );
  procs.lock_and_load                = get_proc( "lock_and_load"                );
  procs.explosive_shot_focus_starved = get_proc( "explosive_shot_focus_starved" );
  procs.black_arrow_focus_starved    = get_proc( "black_arrow_focus_starved"    );
}

// hunter_t::init_rng =======================================================

void hunter_t::init_rng()
{
  player_t::init_rng();

  rngs.invigoration         = get_rng( "invigoration"       );
  rngs.thrill_of_the_hunt   = get_rng( "thrill_of_the_hunt" );

  // Overlapping procs require the use of a "distributed" RNG-stream when normalized_roll=1
  // also useful for frequent checks with low probability of proc and timed effect

  rngs.frenzy               = get_rng( "frenzy" );
  rngs.rabid_power          = get_rng( "rabid_power" );
}

// hunter_t::init_scaling ===================================================

void hunter_t::init_scaling()
{
  player_t::init_scaling();

  scales_with[ STAT_STRENGTH ]         = false;
  scales_with[ STAT_EXPERTISE_RATING ] = false;
}

// hunter_t::init_actions ===================================================

void hunter_t::init_actions()
{
  if ( main_hand_weapon.group() != WEAPON_RANGED )
  {
    sim -> errorf( "Player %s does not have a ranged weapon at the Main Hand slot.", name() );
  }

  if ( action_list_str.empty() )
  {
    clear_action_priority_lists();

    std::string& precombat = get_action_priority_list( "precombat" ) -> action_list_str;

    if ( level >= 80 )
    {
      precombat += "/flask,type=";
      precombat += ( level > 85 ) ? "spring_blossoms" : "winds";

      precombat += "/food,type=";
      precombat += ( level > 85 ) ? "great_pandaren_banquet" : "seafood_magnifique_feast";
    }

    // Todo: Add ranged_vulnerability
    //action_list_str += "/hunters_mark,if=target.time_to_die>=21&!aura.ranged_vulnerability.up";
    precombat += "/summon_pet";
    if ( find_class_spell( "Trueshot Aura" ) -> ok() )
      precombat += "/trueshot_aura";
    precombat += "/snapshot_stats";

    if ( level >= 80 )
    {
      precombat += ( level > 85 ) ? "/virmens_bite_potion" : "/tolvir_potion";

      action_list_str += ( level > 85 ) ? "/virmens_bite_potion" : "/tolvir_potion";
      action_list_str += ",if=buff.bloodlust.react|target.time_to_die<=60";
    }

    if ( specialization() == HUNTER_SURVIVAL )
      action_list_str += init_use_racial_actions();
    action_list_str += init_use_item_actions();
    action_list_str += init_use_profession_actions();
    action_list_str += "/aspect_of_the_hawk,moving=0";
    action_list_str += "/aspect_of_the_fox,moving=1";
    action_list_str += "/auto_shot";
    action_list_str += "/explosive_trap,if=target.adds>0";

    switch ( specialization() )
    {
    // BEAST MASTERY
    case HUNTER_BEAST_MASTERY:
      action_list_str += "/focus_fire,five_stacks=1";
      action_list_str += "/serpent_sting,if=!ticking";

      action_list_str += init_use_racial_actions();
      action_list_str += "/multi_shot,if=target.adds>5";
      action_list_str += "/cobra_shot,if=target.adds>5";
      action_list_str += "/kill_shot";
      action_list_str += "/rapid_fire,if=!buff.bloodlust.up&!buff.beast_within.up";

      if ( talents.a_murder_of_crows -> ok() )
        action_list_str += "/a_murder_of_crows,if=buff.beast_within.up";

      action_list_str += "/kill_command";

      if ( talents.fervor -> ok() )
        action_list_str += "/fervor,if=focus<=37";

      action_list_str += "/arcane_shot,if=focus>=69|buff.beast_within.up";

      if ( level >= 81 )
        action_list_str += "/cobra_shot";
      else
        action_list_str += "/steady_shot";
      break;

    // MARKSMANSHIP
    case HUNTER_MARKSMANSHIP:
      action_list_str += init_use_racial_actions();
      action_list_str += "/multi_shot,if=target.adds>5";
      action_list_str += "/steady_shot,if=target.adds>5";
      action_list_str += "/serpent_sting,if=!ticking&target.health.pct<=90";
      action_list_str += "/chimera_shot,if=target.health.pct<=90";
      action_list_str += "/rapid_fire,if=!buff.bloodlust.up|target.time_to_die<=30";
      if ( talents.readiness -> ok() )
        action_list_str += "/readiness,wait_for_rapid_fire=1";
      action_list_str += "/steady_shot,if=buff.pre_steady_focus.up&buff.steady_focus.remains<3";
      action_list_str += "/kill_shot";
      action_list_str += "/aimed_shot,if=buff.master_marksman_fire.react";

      if ( talents.a_murder_of_crows -> ok() )
        action_list_str += "/a_murder_of_crows";

      if ( set_bonus.tier13_4pc_melee() )
      {
        action_list_str += "/arcane_shot,if=(focus>=66|cooldown.chimera_shot.remains>=4)&(target.health.pct<90&!buff.rapid_fire.up&!buff.bloodlust.react&!buff.berserking.up&!buff.tier13_4pc.react&cooldown.buff_tier13_4pc.remains<=0)";
        action_list_str += "/aimed_shot,if=(cooldown.chimera_shot.remains>5|focus>=80)&(buff.bloodlust.react|buff.tier13_4pc.react|cooldown.buff_tier13_4pc.remains>0)|buff.rapid_fire.up|target.health.pct>90";
      }
      else
      {
        //if ( ! glyphs.arcane_shot -> ok() )
        //  action_list_str += "/aimed_shot,if=cooldown.chimera_shot.remains>5|focus>=80|buff.rapid_fire.up|buff.bloodlust.react|target.health_pct>90";
        action_list_str += "/aimed_shot,if=target.health.pct>90|buff.rapid_fire.up|buff.bloodlust.react";
        if ( race == RACE_TROLL )
          action_list_str += "|buff.berserking.up";
        action_list_str += "/arcane_shot,if=(focus>=66|cooldown.chimera_shot.remains>=5)&(target.health_pct<90&!buff.rapid_fire.up&!buff.bloodlust.react";
        if ( race == RACE_TROLL )
          action_list_str += "&!buff.berserking.up)";
        else
          action_list_str += ")";
      }
      action_list_str += "/steady_shot";
      break;

      // SURVIVAL
    case HUNTER_SURVIVAL:
      action_list_str += "/multi_shot,if=target.adds>2";
      action_list_str += "/cobra_shot,if=target.adds>2";
      action_list_str += "/serpent_sting,if=!ticking&target.time_to_die>=10";
      action_list_str += "/explosive_shot,if=(remains<2.0)";

      if ( talents.a_murder_of_crows -> ok() )
        action_list_str += "/a_murder_of_crows";

      action_list_str += "/kill_shot";
      action_list_str += "/black_arrow,if=target.time_to_die>=8";
      action_list_str += "/rapid_fire";
      action_list_str += "/arcane_shot,if=focus>=67";

      if ( find_class_spell( "Cobra Shot" ) )
        action_list_str += "/cobra_shot";
      else if ( find_class_spell( "Steady Shot" ) )
        action_list_str += "/steady_shot";

      if ( summon_pet_str.empty() )
        summon_pet_str = "cat";
      break;

      // DEFAULT
    default: break;
    }

    if ( summon_pet_str.empty() )
      summon_pet_str = "cat";

    action_list_default = 1;
  }

  player_t::init_actions();
}

void hunter_t::init_items()
{
  player_t::init_items();

  // Check for Vishanka, Jaws of the Earth
  items.size();
  for ( size_t i = 0; i < items.size(); ++i )
  {
    item_t& item = items[ i ];
    if ( item.slot == SLOT_MAIN_HAND && strstr( item.name(), "vishanka_jaws_of_the_earth" ) )
    {
      // Store the spell id, not just if we have it or not
      vishanka = item.heroic() ? 109859 : item.lfr() ? 109857 : 107822;
      uint32_t id = dbc.spell( vishanka ) -> effectN( 1 ).trigger_spell_id();
      active_vishanka = new vishanka_t( this, id );
    }
  }
}
// hunter_t::register_callbacks ==============================================

void hunter_t::register_callbacks()
{
  player_t::register_callbacks();

  if ( mastery.wild_quiver -> ok() )
  {
    attack_t* wq = new wild_quiver_shot_t( this ); // Fixme: Should be created before player_t::init_actions
    callbacks.register_attack_callback( RESULT_ALL_MASK, new wild_quiver_trigger_t( this, wq ) );
    wq -> init();
  }
}

// hunter_t::combat_begin ===================================================

void hunter_t::combat_begin()
{
  player_t::combat_begin();
}

// hunter_t::reset ==========================================================

void hunter_t::reset()
{
  player_t::reset();

  // Active
  active_pet            = 0;
  active_aspect         = ASPECT_NONE;
}

// hunter_t::composite_attack_power_multiplier ==============================

double hunter_t::composite_attack_power_multiplier()
{
  double mult = player_t::composite_attack_power_multiplier();

  if ( buffs.aspect_of_the_hawk -> up() )
  {
    mult *= 1.0 + buffs.aspect_of_the_hawk -> data().effectN( 1 ).percent();
  }

  return mult;
}

// hunter_t::composite_attack_haste =========================================

double hunter_t::composite_attack_haste()
{
  double h = player_t::composite_attack_haste();
  h *= 1.0 / ( 1.0 + buffs.focus_fire -> value() );
  h *= 1.0 / ( 1.0 + buffs.rapid_fire -> value() );
  h *= 1.0 / ( 1.0 + buffs.tier13_4pc -> up() * buffs.tier13_4pc -> data().effectN( 1 ).percent() );
  return h;
}

// hunter_t::composite_player_multiplier ====================================

double hunter_t::composite_player_multiplier( school_e school, action_t* a )
{
  double m = player_t::composite_player_multiplier( school, a );

  if ( school == SCHOOL_NATURE || school == SCHOOL_ARCANE || school== SCHOOL_SHADOW || school == SCHOOL_FIRE )
  {
    m *= 1.0 + mastery.essence_of_the_viper -> effectN( 1 ).coeff() / 100.0 * composite_mastery();
  }
  return m;
}

// hunter_t::matching_gear_multiplier =======================================

double hunter_t::matching_gear_multiplier( attribute_e attr )
{
  if ( attr == ATTR_AGILITY )
    return 0.05;

  return 0.0;
}

// hunter_t::regen  =========================================================

void hunter_t::regen( timespan_t periodicity )
{
  player_t::regen( periodicity );
  periodicity *= 1.0 + current.haste_rating / rating.attack_haste;
  if ( buffs.rapid_fire -> check() && specs.rapid_recuperation -> ok() )
  {
    // 2/4 focus per sec
    double rr_regen = periodicity.total_seconds() * specs.rapid_recuperation -> effectN( 1 ).base_value();

    resource_gain( RESOURCE_FOCUS, rr_regen, gains.rapid_recuperation );
  }
}

// hunter_t::create_options =================================================

void hunter_t::create_options()
{
  player_t::create_options();

  option_t hunter_options[] =
  {
    { "summon_pet", OPT_STRING, &( summon_pet_str  ) },
    { "merge_piercing_shots", OPT_FLT, &( merge_piercing_shots ) },
    { "tier13_4pc_cooldown", OPT_FLT, &( tier13_4pc_cooldown ) },
    { NULL, OPT_UNKNOWN, NULL }
  };

  option_t::copy( options, hunter_options );
}

// hunter_t::create_profile =================================================

bool hunter_t::create_profile( std::string& profile_str, save_e stype, bool save_html )
{
  player_t::create_profile( profile_str, stype, save_html );

#if 0
  for ( pet_t* pet = pet_list; pet; pet = pet -> next_pet )
  {
    hunter_pet_t* cast = ( hunter_pet_t* ) pet;

    if ( pet -> talents_str.empty() )
    {
      for ( int j=0; j < MAX_TALENT_TREES; j++ )
        for ( int k=0; k < ( int ) pet -> talent_trees[ j ].size(); k++ )
          pet -> talents_str += ( char ) ( pet -> talent_trees[ j ][ k ] -> ok() + ( int ) '0' );
    }

    profile_str += "pet=";
    profile_str += util::pet_type_string( cast -> pet_type );
    profile_str += ",";
    profile_str += cast -> name_str + "\n";
    profile_str += "talents=" + cast -> talents_str + "\n";
    profile_str += "active=owner\n";
  }
#endif

  profile_str += "summon_pet=" + summon_pet_str + "\n";

  return true;
}

// hunter_t::copy_from ======================================================

void hunter_t::copy_from( player_t* source )
{
  player_t::copy_from( source );

  hunter_t* p = debug_cast<hunter_t*>( source );

  summon_pet_str = p -> summon_pet_str;
  merge_piercing_shots = p -> merge_piercing_shots;

}

// hunter_t::armory_extensions ==============================================

void hunter_t::armory_extensions( const std::string& /* region */,
                                  const std::string& /* server */,
                                  const std::string& /* character */,
                                  cache::behavior_e  /* caching */ )
{
#if 0
  // Pet support
  static pet_e pet_types[] =
  {
    /* 0*/ PET_NONE,         PET_WOLF,         PET_CAT,          PET_SPIDER,   PET_BEAR,
    /* 5*/ PET_BOAR,         PET_CROCOLISK,    PET_CARRION_BIRD, PET_CRAB,     PET_GORILLA,
    /*10*/ PET_NONE,         PET_RAPTOR,       PET_TALLSTRIDER,  PET_NONE,     PET_NONE,
    /*15*/ PET_NONE,         PET_NONE,         PET_NONE,         PET_NONE,     PET_NONE,
    /*20*/ PET_SCORPID,      PET_TURTLE,       PET_NONE,         PET_NONE,     PET_BAT,
    /*25*/ PET_HYENA,        PET_BIRD_OF_PREY, PET_WIND_SERPENT, PET_NONE,     PET_NONE,
    /*30*/ PET_DRAGONHAWK,   PET_RAVAGER,      PET_WARP_STALKER, PET_SPOREBAT, PET_NETHER_RAY,
    /*35*/ PET_SERPENT,      PET_NONE,         PET_MOTH,         PET_CHIMERA,  PET_DEVILSAUR,
    /*40*/ PET_NONE,         PET_SILITHID,     PET_WORM,         PET_RHINO,    PET_WASP,
    /*45*/ PET_CORE_HOUND,   PET_SPIRIT_BEAST, PET_NONE,         PET_NONE,     PET_NONE,
    /*50*/ PET_FOX,          PET_MONKEY,       PET_DOG,          PET_BEETLE,   PET_NONE,
    /*55*/ PET_SHALE_SPIDER, PET_NONE,         PET_NONE,         PET_NONE,     PET_NONE,
    /*60*/ PET_NONE,         PET_NONE,         PET_NONE,         PET_NONE,     PET_NONE,
    /*65*/ PET_NONE,         PET_WASP,         PET_NONE,         PET_NONE,     PET_NONE
  };
  int num_families = sizeof( pet_types ) / sizeof( pet_e );

  std::string url = "http://" + region + ".battle.net/wow/en/character/" + server + '/' + character + "/pet";
  xml_node_t* pet_xml = xml_t::get( sim, url, caching );
  if ( sim -> debug ) xml_t::print( pet_xml, sim -> output_file );

  xml_node_t* pet_list_xml = xml_t::get_node( pet_xml, "div", "class", "pets-list" );

  xml_node_t* pet_script_xml = xml_t::get_node( pet_list_xml, "script", "type", "text/javascript" );

  if ( ! pet_script_xml )
  {
    sim -> errorf( "Hunter %s unable to download pet data from Armory\n", name() );
    sim -> cancel();
    return;
  }

  std::string cdata_str;
  if ( xml_t::get_value( cdata_str, pet_script_xml, "cdata" ) )
  {
    std::string::size_type pos = cdata_str.find( '{' );
    if ( pos != std::string::npos ) cdata_str.erase( 0, pos+1 );
    pos = cdata_str.rfind( '}' );
    if ( pos != std::string::npos ) cdata_str.erase( pos );

    js_node_t* pet_js = js_t::create( sim, cdata_str );
    pet_js = js_t::get_node( pet_js, "Pet.data" );
    if ( sim -> debug ) js_t::print( pet_js, sim -> output_file );

    if ( ! pet_js )
    {
      sim -> errorf( "\nHunter %s unable to download pet data from Armory\n", name() );
      sim -> cancel();
      return;
    }

    std::vector<js_node_t*> pet_records;
    int num_pets = js_t::get_children( pet_records, pet_js );
    for ( int i=0; i < num_pets; i++ )
    {
      std::string pet_name, pet_talents;
      int pet_level, pet_family;

      if ( ! js_t::get_value( pet_name,    pet_records[ i ], "name"     ) ||
           ! js_t::get_value( pet_talents, pet_records[ i ], "build"    ) ||
           ! js_t::get_value( pet_level,   pet_records[ i ], "level"    ) ||
           ! js_t::get_value( pet_family,  pet_records[ i ], "familyId" ) )
      {
        sim -> errorf( "\nHunter %s unable to decode pet name/build/level/familyId for pet %s\n", name(), pet_name.c_str() );
        continue;
      }

      // Pets can have spaces in names, replace with underscore ..
      for ( unsigned j=0; j < pet_name.length(); j++ )
      {
        if ( pet_name[ j ] == ' ' )
          pet_name[ j ] = '_';
      }

      // Pets can have the same name, so suffix names with an unique
      // identifier from Battle.net, if one is found
      if ( js_t::get_name( pet_records[ i ] ) )
      {
        pet_name += '_';
        pet_name += js_t::get_name( pet_records[ i ] );
      }

      // Pets can have zero talents also, we should probably support it.
      /*
      bool all_zeros = true;
      for ( int j=pet_talents.size()-1; j >=0 && all_zeros; j-- )
        if ( pet_talents[ j ] != '0' )
          all_zeros = false;
      if ( all_zeros ) continue;
      */

      if ( pet_family > num_families || pet_types[ pet_family ] == PET_NONE )
      {
        sim -> errorf( "\nHunter %s unable to decode pet %s family id %d\n", name(), pet_name.c_str(), pet_family );
        continue;
      }

      hunter_pet_t* pet = new hunter_pet_t( sim, this, pet_name, pet_types[ pet_family ] );

      pet -> parse_talents_armory( pet_talents );

      pet -> talents_str.clear();
      for ( int j=0; j < MAX_TALENT_TREES; j++ )
      {
        for ( int k=0; k < ( int ) pet -> talent_trees[ j ].size(); k++ )
        {
          pet -> talents_str += ( char ) ( pet -> talent_trees[ j ][ k ] -> ok() + ( int ) '0' );
        }
      }
    }

    // If we have valid pets, figure out which to summon by parsing Battle.net
    if ( pet_list )
    {
      std::vector<xml_node_t*> pet_nodes;

      int n_pet_nodes = xml_t::get_nodes( pet_nodes, pet_list_xml, "a", "class", "pet" );
      for ( int i = 0; i < n_pet_nodes; i++ )
      {
        xml_node_t* summoned_node = xml_t::get_node( pet_nodes[ i ], "span", "class", "summoned" );
        std::string summoned_pet_name;
        std::string summoned_pet_id;

        if ( ! summoned_node )
          continue;

        xml_t::get_value( summoned_pet_id, pet_nodes[ i ], "data-id" );

        if ( ! xml_t::get_value( summoned_pet_name, xml_t::get_node( pet_nodes[ i ], "span", "class", "name" ), "." ) )
          continue;

        util::html_special_char_decode( summoned_pet_name );
        if ( ! summoned_pet_name.empty() )
        {
          summon_pet_str = summoned_pet_name;
          if ( ! summoned_pet_id.empty() )
            summon_pet_str += '_' + summoned_pet_id;
        }
      }

      // Pick first pet on the list, if no pet is summoned in the battle net profile
      if ( summon_pet_str.empty() )
        summon_pet_str = pet_list -> name_str;
    }
  }
#endif
}

// hunter_t::decode_set =====================================================

int hunter_t::decode_set( item_t& item )
{
  const char* s = item.name();


  if ( item.slot != SLOT_HEAD      &&
       item.slot != SLOT_SHOULDERS &&
       item.slot != SLOT_CHEST     &&
       item.slot != SLOT_HANDS     &&
       item.slot != SLOT_LEGS      )
  {
    return SET_NONE;
  }

  if ( strstr( s, "wyrmstalkers"          ) ) return SET_T13_MELEE;

  if ( strstr( s, "_gladiators_chain_" ) )    return SET_PVP_MELEE;

  return SET_NONE;
}

// hunter_t::moving() =======================================================

void hunter_t::moving()
{
  player_t::interrupt();

  if ( main_hand_attack ) main_hand_attack -> cancel();
  if (  off_hand_attack )  off_hand_attack -> cancel();
}

// hunter_t::set_default_talents() =======================================================

std::string hunter_t::set_default_talents()
{
  switch ( specialization() )
  {
  case SPEC_NONE: break;
  default: break;
  }

  return player_t::set_default_talents();
}

// hunter_t::set_default_glyphs() =======================================================

std::string hunter_t::set_default_glyphs()
{
  switch ( specialization() )
  {
  case SPEC_NONE: break;
  default: break;
  }

  return player_t::set_default_glyphs();
}

// HUNTER MODULE INTERFACE ================================================

struct hunter_module_t : public module_t
{
  hunter_module_t() : module_t( HUNTER ) {}

  virtual player_t* create_player( sim_t* sim, const std::string& name, race_e r = RACE_NONE )
  {
    return new hunter_t( sim, name, r );
  }

  virtual bool valid() { return true; }
  virtual void init        ( sim_t* ) {}
  virtual void combat_begin( sim_t* ) {}
  virtual void combat_end  ( sim_t* ) {}
};

} // ANONYMOUS NAMESPACE

module_t* module_t::hunter()
{
  static module_t* m = 0;
  if ( ! m ) m = new hunter_module_t();
  return m;
}
