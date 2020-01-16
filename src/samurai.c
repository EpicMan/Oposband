#include "angband.h"

static void _tobi_izuna_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Tobi-Izuna");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks a two squares distant monster.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		project_length = 2;
		if (!get_fire_dir(&dir)) break;
		project_hook(GF_ATTACK, dir, HISSATSU_2, PROJECT_STOP | PROJECT_KILL);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _3_way_attack_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "3-Way Attack");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks in 3 directions in one time.");
		break;
	case SPELL_CAST:
	{
		int dir, cdir;
		int y, x;
		var_set_bool(res, FALSE);
		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		for (cdir = 0; cdir < 8; cdir++)
		{
			if (cdd[cdir] == dir) break;
		}

		if (cdir == 8) break;

		y = py + ddy_cdd[cdir];
		x = px + ddx_cdd[cdir];
		if (cave[y][x].m_idx)
			py_attack(y, x, 0);
		else
			msg_print("You attack the empty air.");
		y = py + ddy_cdd[(cdir + 7) % 8];
		x = px + ddx_cdd[(cdir + 7) % 8];
		if (cave[y][x].m_idx)
			py_attack(y, x, 0);
		else
			msg_print("You attack the empty air.");
		y = py + ddy_cdd[(cdir + 1) % 8];
		x = px + ddx_cdd[(cdir + 1) % 8];
		if (cave[y][x].m_idx)
			py_attack(y, x, 0);
		else
			msg_print("You attack the empty air.");
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _boomerang_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Boomerang");
		break;
	case SPELL_DESC:
		var_set_string(res, "Throws current weapon. And it'll return to your hand unless failed.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		py_throw_t context = { 0 };
		context.type = THROW_BOOMERANG;
		context.back_chance = 24 + randint1(5);
		if (!py_throw(&context)) break;
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _burning_strike_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Burning Strike");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks a monster with more damage unless it has resistance to fire.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		int dir, y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_FIRE);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _detect_ferocity_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Detect Ferocity");
		break;
	case SPELL_DESC:
		var_set_string(res, "Detects all monsters except mindless in your vicinity.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		detect_monsters_mind(DETECT_RAD_DEFAULT);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _strike_to_stun_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Strike to Stun");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attempts to stun a monster in the adjacent.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		int dir, y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_MINEUCHI);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _counter_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Counter");
		break;
	case SPELL_DESC:
		var_set_string(res, "Prepares to counterattack. When attack by a monster, strikes back using SP each time.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		if (p_ptr->riding)
		{
			msg_print("You cannot do it when riding.");
			break;
		}
		msg_print("You prepare to counter blow.");
		p_ptr->counter = TRUE; 
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _harainuke_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Harainuke");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks monster with your weapons normally, then move through counter side of the monster.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;
		if (p_ptr->riding)
		{
			msg_print("You cannot do it when riding.");
			break;
		}

		if (!get_rep_dir2(&dir)) break;

		if (dir == 5) break;
		y = py + ddy[dir];
		x = px + ddx[dir];

		if (!cave[y][x].m_idx)
		{
			msg_print("There is no monster.");
			break;
		}

		py_attack(y, x, 0);

		if (!player_can_enter(cave[y][x].feat, 0) || is_trap(cave[y][x].feat))
			break;

		y += ddy[dir];
		x += ddx[dir];

		if (player_can_enter(cave[y][x].feat, 0) && !is_trap(cave[y][x].feat) && !cave[y][x].m_idx)
		{
			msg_print(NULL);
			(void)move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP);
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _serpents_tongue_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Serpent's Tongue");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks a monster with more damage unless it has resistance to poison.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_POISON);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _zammaken_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Zammaken");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks an evil unliving monster with great damage. No effect to other monsters.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_ZANMA);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _wind_blast_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Wind Blast");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks an adjacent monster, and blow it away.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, 0);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
		{
			break;
		}
		if (cave[y][x].m_idx)
		{
			int i;
			int ty = y, tx = x;
			int oy = y, ox = x;
			int m_idx = cave[y][x].m_idx;
			monster_type *m_ptr = &m_list[m_idx];
			char m_name[80];

			monster_desc(m_name, m_ptr, 0);

			for (i = 0; i < 5; i++)
			{
				y += ddy[dir];
				x += ddx[dir];
				if (cave_empty_bold(y, x))
				{
					ty = y;
					tx = x;
				}
				else break;
			}
			if ((ty != oy) || (tx != ox))
			{
				msg_format("You blow %s away!", m_name);
				cave[oy][ox].m_idx = 0;
				cave[ty][tx].m_idx = m_idx;
				m_ptr->fy = ty;
				m_ptr->fx = tx;

				update_mon(m_idx, TRUE);
				lite_spot(oy, ox);
				lite_spot(ty, tx);

				if (r_info[m_ptr->r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
					p_ptr->update |= (PU_MON_LITE);
			}
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _judge_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Judge");
		break;
	case SPELL_DESC:
		var_set_string(res, "Identifies a weapon or armor.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		if (!identify_fully(object_is_weapon_armour_ammo)) break;
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _rock_smash_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Rock Smash");
		break;
	case SPELL_DESC:
		var_set_string(res, "Breaks rock. Or greatly damage a monster made by rocks.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_HAGAN);

		if (!cave_have_flag_bold(y, x, FF_HURT_ROCK)) break;

		cave_alter_feat(y, x, FF_HURT_ROCK);
		p_ptr->update |= (PU_FLOW);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _midare_setsugekka_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Midare-Setsugekka");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks a monster with increased number of attacks and more damage unless it has resistance to cold.");
		break;
	case SPELL_INFO:
		var_set_string(res, info_damage(3 + (p_ptr->lev - 1) / 5, 4, 0));
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_COLD);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _spot_aiming_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Spot Aiming");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attempts to kill a monster instantly. If failed cause only 1HP of damage.");
		break;
	case SPELL_INFO:
		var_set_string(res, info_damage(3 + (p_ptr->lev - 1) / 5, 4, 0));
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_KYUSHO);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _majingiri_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Majingiri");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attempts to attack with critical hit. But this attack is easy to evade for a monster.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_MAJIN);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _desperate_attack_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Desperate Attack");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks with all of your power. But all damages you take will be doubled for one turn.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_SUTEMI);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		p_ptr->sutemi = TRUE;
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _lightning_eagle_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Lightning Eagle");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks a monster with more damage unless it has resistance to electricity.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_ELEC);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _rush_attack_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Rush Attack");
		break;
	case SPELL_DESC:
		var_set_string(res, "Steps close to a monster and attacks at a time.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		if (!rush_attack(5, NULL)) break;
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _bloody_maelstrom_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Bloody Maelstrom");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks all adjacent monsters with power corresponding to your cut status. Then increases your cut status. No effect to unliving monsters.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y = 0, x = 0;

		cave_type       *c_ptr;
		monster_type    *m_ptr;

		if (p_ptr->cut < 300)
			set_cut(p_ptr->cut + 300, FALSE);
		else
			set_cut(p_ptr->cut * 2, FALSE);

		for (dir = 0; dir < 8; dir++)
		{
			y = py + ddy_ddd[dir];
			x = px + ddx_ddd[dir];
			c_ptr = &cave[y][x];

			/* Get the monster */
			m_ptr = &m_list[c_ptr->m_idx];

			/* Hack -- attack monsters */
			if (c_ptr->m_idx && (m_ptr->ml || cave_have_flag_bold(y, x, FF_PROJECT)))
			{
				if (!monster_living(&r_info[m_ptr->r_idx]))
				{
					char m_name[80];

					monster_desc(m_name, m_ptr, 0);
					msg_format("%s is unharmed!", m_name);
				}
				else py_attack(y, x, HISSATSU_SEKIRYUKA);
			}
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _earthquake_blow_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Earthquake Blow");
		break;
	case SPELL_DESC:
		var_set_string(res, "Shakes dungeon structure, and results in random swapping of floors and walls.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_QUAKE);
		else
			earthquake(py, px, 10);

		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _crack_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Crack");
		break;
	case SPELL_DESC:
		var_set_string(res, "Fires a beam of shock wave.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int total_damage = 0, hand;
		if (!get_fire_dir(&dir)) break;
		msg_print("You swing your weapon downward.");

		for (hand = 0; hand < MAX_HANDS; hand++)
		{
			int damage, basedam;
			u32b flgs[OF_ARRAY_SIZE];
			object_type *o_ptr = NULL;

			if (p_ptr->weapon_info[hand].wield_how != WIELD_NONE)
				o_ptr = equip_obj(p_ptr->weapon_info[hand].slot);

			if (!o_ptr) continue;

			basedam = (o_ptr->dd * (o_ptr->ds + 1)) * 50;
			damage = o_ptr->to_h * 100;

			obj_flags(o_ptr, flgs);
			if (have_flag(flgs, OF_VORPAL2))
			{
				basedam *= 5;
				basedam /= 3;
			}
			else if (have_flag(flgs, OF_VORPAL))
			{
				basedam *= 11;
				basedam /= 9;
			}
			damage += basedam;
			damage *= NUM_BLOWS(hand) / 100;
			total_damage += damage / 200;
		}
		fire_beam(GF_FORCE, dir, total_damage);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _war_cry_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "War Cry");
		break;
	case SPELL_DESC:
		var_set_string(res, "Damages all monsters in sight with sound. Aggravate nearby monsters.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		msg_print("You roar out!");
		project_hack(GF_SOUND, randint1(p_ptr->lev * 3));
		aggravate_monsters(0);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _musou_sandan_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Musou-Sandan");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks with powerful 3 strikes.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int i;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		for (i = 0; i < 3; i++)
		{
			int y, x;
			int ny, nx;
			int m_idx;
			cave_type *c_ptr;
			monster_type *m_ptr;

			y = py + ddy[dir];
			x = px + ddx[dir];
			c_ptr = &cave[y][x];

			if (c_ptr->m_idx)
				py_attack(y, x, HISSATSU_3DAN);
			else
			{
				msg_print("There is no monster.");
				break;
			}

			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				break;
			}

			/* Monster is dead? */
			if (!c_ptr->m_idx) break;

			ny = y + ddy[dir];
			nx = x + ddx[dir];
			m_idx = c_ptr->m_idx;
			m_ptr = &m_list[m_idx];

			/* Monster cannot move back? */
			if (!monster_can_enter(ny, nx, &r_info[m_ptr->r_idx], 0))
			{
				/* -more- */
				if (i < 2) msg_print(NULL);
				continue;
			}

			c_ptr->m_idx = 0;
			cave[ny][nx].m_idx = m_idx;
			m_ptr->fy = ny;
			m_ptr->fx = nx;

			update_mon(m_idx, TRUE);

			/* Redraw the old spot */
			lite_spot(y, x);

			/* Redraw the new spot */
			lite_spot(ny, nx);

			/* Player can move forward? */
			if (player_can_enter(c_ptr->feat, 0))
			{
				/* Move the player */
				if (!move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP)) break;
			}
			else
			{
				break;
			}

			/* -more- */
			if (i < 2) msg_print(NULL);
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _vampires_fang_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Vampire's Fang");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks with vampiric strikes which absorbs HP from a monster and gives them to you. No effect to unliving monsters.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_DRAIN);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _moon_dazzling_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Moon Dazzling");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attempts to stun, confuse and sleep all waking monsters.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		msg_print("You irregularly wave your weapon...");
		project_hack(GF_ENGETSU, p_ptr->lev * 4);
		project_hack(GF_ENGETSU, p_ptr->lev * 4);
		project_hack(GF_ENGETSU, p_ptr->lev * 4);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _hundred_slaughter_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Hundred Slaughter");
		break;
	case SPELL_DESC:
		var_set_string(res, "Performs a series of rush attacks. The series continues while killing each monster in a time and SP remains.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		const int mana_cost_per_monster = 8;
		bool new = TRUE;
		bool mdeath;

		do
		{
			if (!rush_attack(5, &mdeath)) break;
			if (new)
			{
				/* Reserve needed mana point */
				p_ptr->csp -= technic_info[REALM_HISSATSU - MIN_TECHNIC][26].smana;
				new = FALSE;
			}
			else
				p_ptr->csp -= mana_cost_per_monster;

			if (!mdeath) break;
			command_dir = 0;

			p_ptr->redraw |= PR_MANA;
			handle_stuff();
		} while (p_ptr->csp > mana_cost_per_monster);

		if (new) break;

		/* Restore reserved mana */
		p_ptr->csp += technic_info[REALM_HISSATSU - MIN_TECHNIC][26].smana;
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _dragonic_flash_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Dragonic Flash");
		break;
	case SPELL_DESC:
		var_set_string(res, "Runs toward given location while attacking all monsters on the path.");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		int y, x;

		if (!tgt_pt(&x, &y, MAX_SIGHT / 2)) break;

		if (!cave_player_teleportable_bold(y, x, 0L) ||
			(distance(y, x, py, px) > MAX_SIGHT / 2) ||
			!projectable(py, px, y, x))
		{
			msg_print("You cannot move to that place!");
			break;
		}
		if (p_ptr->anti_tele)
		{
			msg_print("A mysterious force prevents you from teleporting!");
			equip_learn_flag(OF_NO_TELE);
			break;
		}
		project(0, 0, y, x, HISSATSU_ISSEN, GF_ATTACK, PROJECT_BEAM | PROJECT_KILL);
		teleport_player_to(y, x, 0L);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _twin_slash_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Twin Slash");
		break;
	case SPELL_DESC:
		var_set_string(res, "Double attacks at a time.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int x, y;

		if (!get_rep_dir(&dir, FALSE)) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
		{
			py_attack(y, x, 0);
			if (cave[y][x].m_idx)
			{
				handle_stuff();
				py_attack(y, x, 0);
			}
		}
		else
		{
			msg_print("You don't see any monster in this direction");
			break;
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _kofuku_zettousei_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Kofuku-Zettousei");
		break;
	case SPELL_DESC:
		var_set_string(res, "Performs a powerful attack which even effect nearby monsters.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int total_damage = 0, hand;
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
		{
			msg_print("Something prevent you from attacking.");
			break;
		}
		msg_print("You swing your weapon downward.");
		for (hand = 0; hand < MAX_HANDS; hand++)
		{
			int damage, basedam;
			u32b flgs[OF_ARRAY_SIZE];
			object_type *o_ptr = NULL;

			if (p_ptr->weapon_info[hand].wield_how != WIELD_NONE)
				o_ptr = equip_obj(p_ptr->weapon_info[hand].slot);

			if (!o_ptr) continue;

			basedam = (o_ptr->dd * (o_ptr->ds + 1)) * 50;
			damage = o_ptr->to_h * 100;

			obj_flags(o_ptr, flgs);
			if (have_flag(flgs, OF_VORPAL2))
			{
				basedam *= 5;
				basedam /= 3;
			}
			else if (have_flag(flgs, OF_VORPAL))
			{
				basedam *= 11;
				basedam /= 9;
			}
			damage += basedam;
			damage += p_ptr->weapon_info[hand].to_d * 100;
			damage *= NUM_BLOWS(hand) / 100;
			total_damage += damage / 200;
		}
		project(0, (cave_have_flag_bold(y, x, FF_PROJECT) ? 5 : 0), y, x, total_damage * 3 / 2, GF_METEOR, PROJECT_KILL | PROJECT_JUMP | PROJECT_ITEM);
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _keiun_kininken_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Keiun-Kininken");
		break;
	case SPELL_DESC:
		var_set_string(res, "Attacks a monster with extremely powerful damage. But you also takes some damages. Hurts a undead monster greatly.");
		break;
	case SPELL_CAST:
	{
		int dir;
		var_set_bool(res, FALSE);
		int y, x;

		if (!get_rep_dir2(&dir)) break;
		if (dir == 5) break;

		y = py + ddy[dir];
		x = px + ddx[dir];

		if (cave[y][x].m_idx)
			py_attack(y, x, HISSATSU_UNDEAD);
		else
		{
			msg_print("There is no monster.");
			break;
		}
		take_hit(DAMAGE_NOESCAPE, 100 + randint1(100), "exhaustion on using Keiun-Kininken");
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

static void _harakiri_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Harakiri");
		break;
	case SPELL_DESC:
		var_set_string(res, "'Busido is found in death'");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		int i;
		if (!get_check("Do you really want to commit suicide? ")) break;
		/* Special Verification for suicide */
		prt("Please verify SUICIDE by typing the '@' sign: ", 0, 0);

		flush();
		i = inkey();
		prt("", 0, 0);
		if (i != '@') break;
		if (p_ptr->total_winner)
		{
			take_hit(DAMAGE_FORCE, 9999, "Seppuku");
			p_ptr->total_winner = TRUE;
		}
		else
		{
			msg_print("Meaning of Bushi-do is found in the death.");
			take_hit(DAMAGE_FORCE, 9999, "Seppuku");
		}
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

/* Virtual spellbooks for the samurai
*/
/* level, cost, fail chance, function*/
static book_t _books[4] = {
    { "Bugei Shofu",
		{{ 1, 15, 0, _tobi_izuna_spell },
		 { 3, 10, 0, _3_way_attack_spell },
		 { 6, 15, 0, _boomerang_spell },
		 { 9,  8, 0, _burning_strike_spell },
		 {10, 12, 0, _detect_ferocity_spell },
		 {12, 25, 0, _strike_to_stun_spell },
		 {14,  7, 0, _counter_spell },
		 {17, 20, 0, _harainuke_spell }}
    },
    { "Yagyuu Bugeichou",
        {{ 19, 10, 0, _serpents_tongue_spell },
		{ 22, 20, 0, _zammaken_spell },
		{ 24, 30, 0, _wind_blast_spell },
		{ 25, 10, 0, _judge_spell },
		{ 27, 15, 0, _rock_smash_spell },
		{ 29, 45, 0, _midare_setsugekka_spell },
		{ 32, 70, 0, _spot_aiming_spell },
		{ 35, 50, 0, _majingiri_spell }}
    },
	/* DL45 item, assume found by 45/2 = level 22 character */
    { "Gorinnosho",
		{ { 22, 40, 0, _desperate_attack_spell },
		{ 24, 22, 0, _lightning_eagle_spell },
		{ 26, 30, 0, _rush_attack_spell },
		{ 28, 35, 0, _bloody_maelstrom_spell },
		{ 32, 30, 0, _earthquake_blow_spell },
		{ 34, 60, 0, _crack_spell },
		{ 37, 40, 0, _war_cry_spell },
		{ 39, 80, 0, _musou_sandan_spell } }
    },
	/* DL80 item, assume found by level 40 char */
    { "Hokusin Ittouryuu Kaiden",
		 { { 40, 20, 0, _vampires_fang_spell },
		 { 40, 40, 0, _moon_dazzling_spell },
		 { 41, 35, 0, _hundred_slaughter_spell },
		 { 42, 80, 0, _dragonic_flash_spell },
		 { 43, 100, 0, _twin_slash_spell },
		 { 44, 110, 0, _kofuku_zettousei_spell },
		 { 45, 130, 0, _keiun_kininken_spell },
		 { 50, 255, 0, _harakiri_spell } }
    },
};

static int _spell_index(int book, int spell)
{
    return book * _SPELLS_PER_BOOK + spell;
}

static bool _is_spell_known(int book, int spell)
{
    int idx = _spell_index(book, spell);

	if (_books[book].spells[spell].level <= p_ptr->lev)
	{
		return TRUE;
	}
    else
		return FALSE;
}


void samurai_browse_spell(void)
{
    /* TODO: Perhaps browse should display contents of rage
       spellbooks in inventory rather than already known spells? */
    do_cmd_spell_browse();
}

void samurai_posture_calc_bonuses(void)
{
	if (p_ptr->special_defense & KATA_FUUJIN)
	{
		/* see project_p for special handling ... review?
		if (!p_ptr->blind)
		p_ptr->reflect = TRUE; */
	}
	if (p_ptr->special_defense & KATA_KOUKIJIN)
	{
		p_ptr->to_a -= 50;
		p_ptr->dis_to_a -= 50;
		res_add_vuln(RES_ACID);
		res_add_vuln(RES_ELEC);
		res_add_vuln(RES_FIRE);
		res_add_vuln(RES_COLD);
	}

	if (p_ptr->special_defense & KATA_MUSOU)
	{
		p_ptr->see_inv++;
		p_ptr->free_act++;
		p_ptr->slow_digest = TRUE;
		p_ptr->regen += 100;
		p_ptr->levitation = TRUE;
		p_ptr->hold_life++;
		p_ptr->sustain_str = TRUE;
		p_ptr->sustain_int = TRUE;
		p_ptr->sustain_wis = TRUE;
		p_ptr->sustain_con = TRUE;
		p_ptr->sustain_dex = TRUE;
		p_ptr->sustain_chr = TRUE;
		p_ptr->telepathy = TRUE;
		p_ptr->lite = TRUE;
		res_add_all();
		p_ptr->reflect = TRUE;
		p_ptr->sh_fire++;
		p_ptr->sh_elec++;
		p_ptr->sh_cold++;
		p_ptr->to_a += 100;
		p_ptr->dis_to_a += 100;
	}
}

void samurai_posture_calc_stats(s16b stats[MAX_STATS])
{
	if (p_ptr->special_defense & KATA_KOUKIJIN)
	{
		int i;
		for (i = 0; i < MAX_STATS; i++)
			stats[i] += 5;
	}
}

void samurai_posture_get_flags(u32b flgs[OF_ARRAY_SIZE])
{
	if (p_ptr->special_defense & KATA_FUUJIN)
		add_flag(flgs, OF_REFLECT);

	if (p_ptr->special_defense & KATA_MUSOU)
	{
		add_flag(flgs, OF_RES_FEAR);
		add_flag(flgs, OF_RES_LITE);
		add_flag(flgs, OF_RES_DARK);
		add_flag(flgs, OF_RES_BLIND);
		add_flag(flgs, OF_RES_CONF);
		add_flag(flgs, OF_RES_SOUND);
		add_flag(flgs, OF_RES_SHARDS);
		add_flag(flgs, OF_RES_NETHER);
		add_flag(flgs, OF_RES_NEXUS);
		add_flag(flgs, OF_RES_CHAOS);
		add_flag(flgs, OF_RES_DISEN);
		add_flag(flgs, OF_RES_TIME);
		add_flag(flgs, OF_REFLECT);
		add_flag(flgs, OF_HOLD_LIFE);
		add_flag(flgs, OF_FREE_ACT);
		add_flag(flgs, OF_AURA_FIRE);
		add_flag(flgs, OF_AURA_ELEC);
		add_flag(flgs, OF_AURA_COLD);
		add_flag(flgs, OF_LEVITATION);
		add_flag(flgs, OF_LITE);
		add_flag(flgs, OF_SEE_INVIS);
		add_flag(flgs, OF_TELEPATHY);
		add_flag(flgs, OF_SLOW_DIGEST);
		add_flag(flgs, OF_REGEN);
		add_flag(flgs, OF_SUST_STR);
		add_flag(flgs, OF_SUST_INT);
		add_flag(flgs, OF_SUST_WIS);
		add_flag(flgs, OF_SUST_DEX);
		add_flag(flgs, OF_SUST_CON);
		add_flag(flgs, OF_SUST_CHR);
	}

	if (p_ptr->special_defense & KATA_KOUKIJIN)
	{
		add_flag(flgs, OF_VULN_ACID);
		add_flag(flgs, OF_VULN_ELEC);
		add_flag(flgs, OF_VULN_FIRE);
		add_flag(flgs, OF_VULN_COLD);
	}
}

static bool _choose_kata(void)
{
	char choice;
	int new_kata = 0;
	int i;
	char buf[80];

	if (p_ptr->confused)
	{
		msg_print("You are too confused.");
		return FALSE;
	}
	if (p_ptr->stun)
	{
		msg_print("You are not clear headed");
		return FALSE;
	}
	if (p_ptr->afraid)
	{
		msg_print("You are trembling with fear!");
		return FALSE;
	}

	screen_save();

	prt(" a) No Form", 2, 20);

	for (i = 0; i < MAX_KATA; i++)
	{
		if (p_ptr->lev >= kata_shurui[i].min_level)
		{
			sprintf(buf, " %c) Form of %-12s  %s", I2A(i + 1), kata_shurui[i].desc, kata_shurui[i].info);
			prt(buf, 3 + i, 20);
		}
	}

	prt("", 1, 0);
	prt("        Choose Form: ", 1, 14);

	for (;;)
	{
		choice = inkey();

		if (choice == ESCAPE)
		{
			screen_load();
			return FALSE;
		}
		else if ((choice == 'a') || (choice == 'A'))
		{
			if (p_ptr->action == ACTION_KATA)
				set_action(ACTION_NONE);
			else
				msg_print("You are not assuming posture.");

			screen_load();
			return TRUE;
		}
		else if ((choice == 'b') || (choice == 'B'))
		{
			new_kata = 0;
			break;
		}
		else if (((choice == 'c') || (choice == 'C')) && (p_ptr->lev > 29))
		{
			new_kata = 1;
			break;
		}
		else if (((choice == 'd') || (choice == 'D')) && (p_ptr->lev > 34))
		{
			new_kata = 2;
			break;
		}
		else if (((choice == 'e') || (choice == 'E')) && (p_ptr->lev > 39))
		{
			new_kata = 3;
			break;
		}
	}
	set_action(ACTION_KATA);

	if (p_ptr->special_defense & (KATA_IAI << new_kata))
	{
		msg_print("You reassume a posture.");
	}
	else
	{
		p_ptr->special_defense &= ~(KATA_MASK);
		p_ptr->update |= (PU_BONUS);
		p_ptr->update |= (PU_MONSTERS);
		msg_format("You assume a posture of %s form.", kata_shurui[new_kata].desc);
		p_ptr->special_defense |= (KATA_IAI << new_kata);
	}
	p_ptr->redraw |= (PR_STATE);
	p_ptr->redraw |= (PR_STATUS);
	screen_load();
	return TRUE;
}

static int _max_sp(void)
{
	return MAX(p_ptr->msp * 4, p_ptr->lev * 5 + 5);
}

void cast_concentration(void)
{
	int max_csp = _max_sp();
	if (total_friends)
		return;
	if (p_ptr->special_defense & KATA_MASK)
		return;

	msg_print("You concentrate to charge your power.");

	p_ptr->csp += p_ptr->msp / 2;
	if (p_ptr->csp >= max_csp)
	{
		p_ptr->csp = max_csp;
		p_ptr->csp_frac = 0;
	}

	p_ptr->redraw |= (PR_MANA);
}

void samurai_concentration_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Concentration");
		break;
	case SPELL_DESC:
		var_set_string(res, "");
		break;
	case SPELL_CAST:
	{
		var_set_bool(res, FALSE);
		if (total_friends)
		{
			msg_print("You need concentration on the pets now.");
			return;
		}
		if (p_ptr->special_defense & KATA_MASK)
		{
			msg_print("You need concentration on your form.");
			return;
		}

		cast_concentration();
		var_set_bool(res, TRUE);
		break;
	}
	default:
		default_spell(cmd, res);
		break;
	}
}

void samurai_posture_spell(int cmd, variant *res)
{
	switch (cmd)
	{
	case SPELL_NAME:
		var_set_string(res, "Assume a Guard Position");
		break;
	case SPELL_DESC:
		var_set_string(res, "");
		break;
	case SPELL_CAST:
		var_set_bool(res, FALSE);
		if (!equip_find_first(object_is_melee_weapon))
		{
			msg_print("You need to wield a weapon.");
			return;
		}
		if (!_choose_kata()) return;

		p_ptr->update |= (PU_BONUS);
		var_set_bool(res, TRUE);
		break;
	default:
		default_spell(cmd, res);
		break;
	}
}



static caster_info * _caster_info(void)
{
    static caster_info me = {0};
    static bool init = FALSE;
    if (!init)
    {
        me.magic_desc = "technique";
		me.which_stat = A_WIS;
        me.encumbrance.max_wgt = 3000;
        me.encumbrance.weapon_pct = 0;
        me.encumbrance.enc_wgt = 1200;
		me.options = CASTER_SUPERCHARGE_MANA;
        init = TRUE;
    }
    return &me;
}

static void _calc_bonuses(void)
{
	samurai_posture_calc_bonuses();
	if (p_ptr->lev >= 30)
		res_add(RES_FEAR);
}

static void _calc_stats(s16b stats[MAX_STATS])
{
	samurai_posture_calc_stats(stats);
}

static int _get_spells_imp(spell_info* spells, int max, int book, bool total_skip)
{
    int ct = 0, skip = 0, i;
    for (i = 0; i < _SPELLS_PER_BOOK; i++)
    {
        spell_info *src, *dest;

        if (ct >= max) break;
        src = &_books[book].spells[i];

        if ((!total_skip) || (_is_spell_known(book, i)))
        {
            dest = &spells[ct++];
            dest->level = src->level;
            dest->cost = src->cost;
            dest->fail = calculate_fail_rate(src->level, src->fail, p_ptr->stat_ind[A_STR]);
            dest->fn = src->fn;
        }
    }
    return (ct - skip);
}

static void _book_menu_fn(int cmd, int which, vptr cookie, variant *res)
{
    switch (cmd)
    {
    case MENU_TEXT:
        var_set_string(res, _books[which].name);
        break;
    default:
        default_menu(cmd, which, cookie, res);
    }
}

static int _get_spells(spell_info* spells, int max)
{
    int idx = -1;
    int ct = 0;
    menu_t menu = { "Use which group?", NULL, NULL,
                    _book_menu_fn, _books, 4, 0 };

    idx = menu_choose(&menu);
    if (idx < 0) return 0;

    ct = _get_spells_imp(spells, max, idx, FALSE);
    if (ct == 0)
    {
        msg_print("You don't know any of those techniques yet!");
        return 0;
    }
    return 8;
}

static void _get_flags(u32b flgs[OF_ARRAY_SIZE])
{
	samurai_posture_get_flags(flgs);
	if (p_ptr->lev >= 30)
		add_flag(flgs, OF_RES_FEAR);
}

static int _get_powers(spell_info* spells, int max)
{
	int ct = 0;

	spell_info* spell = &spells[ct++];
	spell->level = 1;
	spell->cost = 0;
	spell->fail = 0;
	spell->fn = samurai_concentration_spell;

	spell = &spells[ct++];
	spell->level = 25;
	spell->cost = 0;
	spell->fail = 0;
	spell->fn = samurai_posture_spell;

	return ct;
}


static void _character_dump(doc_ptr doc)
{
    spell_info spells[MAX_SPELLS];
    int        ct = 0, i;

    for (i = 0; i < 4; i++)
        ct += _get_spells_imp(spells + ct, MAX_SPELLS - ct, i, TRUE);

    py_display_spells(doc, spells, ct);
}

static void _birth(void)
{
	py_birth_obj_aux(TV_SWORD, SV_KATANA, 1);
	py_birth_obj_aux(TV_HARD_ARMOR, SV_CHAIN_MAIL, 1);

	for (int i = PROF_BLUNT; i < PROF_DAGGER; i++)
		p_ptr->proficiency[i] = WEAPON_EXP_BEGINNER;

	for (int i = PROF_BLUNT; i < PROF_DAGGER; i++)
		p_ptr->proficiency_cap[i] = WEAPON_EXP_MASTER;

	p_ptr->proficiency_cap[PROF_BOW] = WEAPON_EXP_SKILLED;
	p_ptr->proficiency_cap[PROF_CROSSBOW] = WEAPON_EXP_SKILLED;
	p_ptr->proficiency_cap[PROF_DUAL_WIELDING] = WEAPON_EXP_MASTER;
	p_ptr->proficiency_cap[PROF_RIDING] = RIDING_EXP_SKILLED;
}

class_t *samurai_get_class(void)
{
	static class_t me = { 0 };
	static bool init = FALSE;

	if (!init)
	{           /* dis, dev, sav, stl, srh, fos, thn, thb */
		skills_t bs = { 25,  18,  32,   2,  16,   6,  70,  40 };
		skills_t xs = { 12,   7,  10,   0,   0,   0,  23,  18 };


		me.name = "Samurai";
		me.desc = "Samurai, masters of the art of the blade, are the next strongest "
			"fighters after Warriors, and can use various special combat "
			"techniques. Samurai are not good at most other skills, and many "
			"magical devices may be too difficult for them to use. Wisdom "
			"determines a Samurai's ability to use the special combat "
			"techniques available to him.\n \n"
			"Samurai use the art of the blade called Kendo (or Bugei). Books "
			"of Kendo are similar to spellbooks, but Samurai don't need to "
			"carry them around; the books are needed only when they study new "
			"combat techniques. Samurai need a weapon wielded to use the "
			"techniques of Kendo, and most techniques will add powerful special "
			"properties to their blows; such as flaming, poisoning, vampiric, "
			"etc... Their maximum spellpoints don't depend on their level but "
			"solely on wisdom, and they can use the class power 'Concentration' "
			"to temporarily increase SP beyond its usual maximum value. They "
			"have one more class power - 'Assume a Posture'. They can choose "
			"different forms of posture in different situations.";

		me.stats[A_STR] = 3;
		me.stats[A_INT] = -2;
		me.stats[A_WIS] = 1;
		me.stats[A_DEX] = 2;
		me.stats[A_CON] = 1;
		me.stats[A_CHR] = 0;
		me.base_skills = bs;
		me.extra_skills = xs;
		me.life = 109;
		me.base_hp = 12;
		me.exp = 130;
		me.pets = 40;
		me.flags = CLASS_SENSE1_FAST | CLASS_SENSE1_STRONG;

		me.birth = _birth;
		me.caster_info = _caster_info;
		me.get_spells = _get_spells;
		me.calc_bonuses = _calc_bonuses;
		me.calc_stats = _calc_stats;
		me.get_flags = _get_flags;
		me.get_powers = _get_powers;
		me.character_dump = spellbook_character_dump;
		init = TRUE;
	}

	return &me;
}

bool samurai_can_concentrate(void)
{
	caster_info *caster_ptr = get_caster_info();
	if (!(caster_ptr && (caster_ptr->options & CASTER_SUPERCHARGE_MANA))) return FALSE;
	if (p_ptr->csp >= _max_sp()) return FALSE;
	return TRUE;
}