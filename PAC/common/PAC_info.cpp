#include <string.h>

#include "PAC_info.h"
#include "PAC_err.h"

#include "lua_manager.h"

#include "bus_coupler_io.h"

auto_smart_ptr < PAC_info > PAC_info::instance;///< ��������� ������.

const u_int_4 PAC_info::MSEC_IN_DAY = 24UL * 60UL * 60UL * 1000UL;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
PAC_info::PAC_info() :
    par( saved_params_u_int_4( P_PARAMS_COUNT ) ),
    up_days( 0 ),
    up_hours( 0 ),
    up_mins( 0 ),
    up_secs( 0 ),
    up_msec( 0 ),
    last_check_time( get_millisec() ),
    reset_type( 1 ), //+ IsResetByWatchDogTimer()
    cmd( 0 ),
    restrictions_set_to_off_time( 0 )
    {
    strcpy( up_time_str, "0 ��. 0:0:0" );
    cmd_answer[ 0 ] = 0;
    }
//-----------------------------------------------------------------------------
PAC_info::~PAC_info()
    {
    }
//-----------------------------------------------------------------------------
void PAC_info::eval()
    {
    if ( restrictions_set_to_off_time &&
        get_delta_millisec( restrictions_set_to_off_time ) >
        par[ P_RESTRICTIONS_MANUAL_TIME ] )
        {
        par[ P_RESTRICTIONS_MODE ] = 0;
        restrictions_set_to_off_time = 0;
        }

    if ( get_delta_millisec( last_check_time ) > 1000 )
        {
        up_msec += get_delta_millisec( last_check_time );
        if ( up_msec >= MSEC_IN_DAY )
            {
            up_days++;
            up_msec %= MSEC_IN_DAY;
            }
        last_check_time = get_millisec();

        up_hours = up_msec / ( 1000UL * 60 * 60 );
        up_mins = up_msec / ( 1000UL * 60 ) % 60;
        up_secs = up_msec / 1000 % 60;

        sprintf( up_time_str, "%lu ��. %02lu:%02lu:%02lu",
            (u_long)up_days, (u_long)up_hours,
            (u_long)up_mins, (u_long)up_secs );
        }
    }
//-----------------------------------------------------------------------------
void PAC_info::reset_params()
    {
    par[ P_MIX_FLIP_PERIOD ] = 60;
    par[ P_MIX_FLIP_UPPER_TIME ] = 1000;
    par[ P_MIX_FLIP_LOWER_TIME ] = 1000;
    par[ P_V_OFF_DELAY_TIME ] = 1000;
    par[ P_V_BOTTOM_OFF_DELAY_TIME ] = 1200;

    par[ P_WAGO_TCP_NODE_WARN_ANSWER_AVG_TIME ] = 50;
    par[ P_MAIN_CYCLE_WARN_ANSWER_AVG_TIME ] = 300;

    par[ P_RESTRICTIONS_MODE ] = 0;
    par[ P_RESTRICTIONS_MANUAL_TIME ] = 2 * 60 * 1000; // 2 min

    par[ P_RESTRICTIONS_MANUAL_TIME ] = 0;
    par[ P_AUTO_PAUSE_OPER_ON_DEV_ERR ] = 0;

    par.save_all();
    }
//-----------------------------------------------------------------------------
int PAC_info::save_device( char* buff )
    {
    int answer_size = sprintf( buff, "t.SYSTEM = \n\t{\n" );

    answer_size += sprintf( buff + answer_size, "\tRESET_BY=%u,\n", reset_type );
    answer_size += sprintf( buff + answer_size, "\tUP_DAYS=%u,\n", up_days );
    answer_size += sprintf( buff + answer_size, "\tUP_HOURS=%u,\n", up_hours );
    answer_size += sprintf( buff + answer_size, "\tUP_MINS=%u,\n", up_mins );
    answer_size += sprintf( buff + answer_size, "\tUP_SECS=%u,\n", up_secs );
    answer_size += sprintf( buff + answer_size, "\tUP_TIME=\"%s\",\n",
        up_time_str );

    answer_size += sprintf( buff + answer_size, "\tWASH_VALVE_SEAT_PERIOD=%d,\n",
        par[ P_MIX_FLIP_PERIOD ] );
    answer_size += sprintf( buff + answer_size, "\tWASH_VALVE_UPPER_SEAT_TIME=%d,\n",
        par[ P_MIX_FLIP_UPPER_TIME ] );
    answer_size += sprintf( buff + answer_size, "\tWASH_VALVE_LOWER_SEAT_TIME=%d,\n",
        par[ P_MIX_FLIP_LOWER_TIME ] );

    answer_size += sprintf( buff + answer_size, "\tP_V_OFF_DELAY_TIME=%d,\n",
        par[ P_V_OFF_DELAY_TIME ] );
    answer_size += sprintf( buff + answer_size, "\tP_V_BOTTOM_ON_DELAY_TIME=%d,\n",
        par[ P_V_BOTTOM_OFF_DELAY_TIME ] );

    answer_size += sprintf( buff + answer_size,
        "\tP_WAGO_TCP_NODE_WARN_ANSWER_AVG_TIME=%d,\n",
        par[ P_WAGO_TCP_NODE_WARN_ANSWER_AVG_TIME ] );
    answer_size += sprintf( buff + answer_size,
        "\tP_MAIN_CYCLE_WARN_ANSWER_AVG_TIME=%d,\n",
        par[ P_MAIN_CYCLE_WARN_ANSWER_AVG_TIME ] );

    answer_size += sprintf( buff + answer_size,
        "\tP_RESTRICTIONS_MODE=%d,\n", par[ P_RESTRICTIONS_MODE ] );
    answer_size += sprintf( buff + answer_size,
        "\tP_RESTRICTIONS_MANUAL_TIME=%d,\n", par[ P_RESTRICTIONS_MANUAL_TIME ] );

    answer_size += sprintf( buff + answer_size,
        "\tP_AUTO_PAUSE_OPER_ON_DEV_ERR=%d,\n",
        par[ P_AUTO_PAUSE_OPER_ON_DEV_ERR ] );

    answer_size += sprintf( buff + answer_size,
        "\tCMD=%d,\n", cmd );
    answer_size += sprintf( buff + answer_size,
        "\tCMD_ANSWER=\"%s\",\n", cmd_answer );

    answer_size += sprintf( buff + answer_size, "\tNODEENABLED = \n\t{\n\t" );

    unsigned int nc = io_manager::get_instance()->get_nodes_count();
    for ( unsigned int i = 0; i < nc; i++ )
        {
        io_manager::io_node* wn = io_manager::get_instance()->get_node( i );
        answer_size += sprintf( buff + answer_size, wn->is_active ? "1, " : "0, " );
        }
    answer_size += sprintf( buff + answer_size, "\n\t},\n" );

    answer_size += sprintf( buff + answer_size, "\t}\n" );

    return answer_size;
    }
//-----------------------------------------------------------------------------
PAC_info* PAC_info::get_instance()
    {
    if ( instance.is_null() )
        {
        instance = new PAC_info();
        }

    return instance;
    }
//-----------------------------------------------------------------------------
void PAC_info::print() const
    {
    printf( "PAC_info\n" );
    }
//-----------------------------------------------------------------------------
int PAC_info::set_cmd( const char* prop, u_int idx, double val )
    {
    if ( strcmp( prop, "CMD" ) == 0 )
        {        
        switch ((COMMANDS)(int)val)
            {
            case RELOAD_RESTRICTIONS:
                {
                if (G_DEBUG)
                    {
                    G_LOG->notice("Reload restrictions (remote monitor client command).");
                    }
                const int SCRIPT_N =
#if defined RM_PAC
                    9;
#else   
                    8;
#endif // defined RM_PAC

                cmd = G_LUA_MANAGER->reload_script(SCRIPT_N, "restrictions",
                    cmd_answer, sizeof(cmd_answer));
                }
                break;

            case RESET_PARAMS:
                if ( G_DEBUG )
                    {
                    G_LOG->notice( "Resetting params (remote monitor client command)." );
                    }
                params_manager::get_instance()->reset_params_size();
                params_manager::get_instance()->final_init();
                break;
            }

        return 0;
        }

    if ( strcmp( prop, "WASH_VALVE_SEAT_PERIOD" ) == 0 )
        {
        par.save( P_MIX_FLIP_PERIOD, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "WASH_VALVE_UPPER_SEAT_TIME" ) == 0 )
        {
        par.save( P_MIX_FLIP_UPPER_TIME, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "WASH_VALVE_LOWER_SEAT_TIME" ) == 0 )
        {
        par.save( P_MIX_FLIP_LOWER_TIME, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "P_V_OFF_DELAY_TIME" ) == 0 )
        {
        par.save( P_V_OFF_DELAY_TIME, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "P_V_BOTTOM_ON_DELAY_TIME" ) == 0 )
        {
        par.save( P_V_BOTTOM_OFF_DELAY_TIME, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "P_WAGO_TCP_NODE_WARN_ANSWER_AVG_TIME" ) == 0 )
        {
        par.save( P_WAGO_TCP_NODE_WARN_ANSWER_AVG_TIME, (u_int_4)val );
        return 0;
        }
    if ( strcmp( prop, "P_MAIN_CYCLE_WARN_ANSWER_AVG_TIME" ) == 0 )
        {
        par.save( P_MAIN_CYCLE_WARN_ANSWER_AVG_TIME, (u_int_4)val );
        return 0;
        }


    if ( strcmp( prop, "P_RESTRICTIONS_MODE" ) == 0 )
        {
        par.save( P_RESTRICTIONS_MODE, (u_int_4)val );

        if ( val == 2 ) //������������������ �����.
            {
            restrictions_set_to_off_time = get_millisec();
            }
        else
            {
            restrictions_set_to_off_time = 0;
            }

        return 0;
        }
    if ( strcmp( prop, "P_RESTRICTIONS_MANUAL_TIME" ) == 0 )
        {
        par.save( P_RESTRICTIONS_MANUAL_TIME, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "P_AUTO_PAUSE_OPER_ON_DEV_ERR" ) == 0 )
        {
        par.save( P_AUTO_PAUSE_OPER_ON_DEV_ERR, (u_int_4)val );
        return 0;
        }

    if ( strcmp( prop, "NODEENABLED" ) == 0 )
        {
        if ( idx <= io_manager::get_instance()->get_nodes_count() )
            {
            io_manager::io_node* wn = io_manager::get_instance()->get_node( idx - 1 );
            if ( 1 == val )
                {
                if ( !wn->is_active )
                    {
                    wn->is_active = 1;
                    }
                PAC_critical_errors_manager::get_instance()->reset_global_error(
                    PAC_critical_errors_manager::AC_SERVICE,
                    PAC_critical_errors_manager::AS_IO_COUPLER, wn->number );
                }
            if ( 0 == val )
                {
                if ( wn->is_active )
                    {
                    io_manager::get_instance()->disconnect( wn );
                    wn->is_active = 0;
                    }
                PAC_critical_errors_manager::get_instance()->set_global_error(
                    PAC_critical_errors_manager::AC_SERVICE,
                    PAC_critical_errors_manager::AS_IO_COUPLER, wn->number );
                PAC_critical_errors_manager::get_instance()->reset_global_error(
                    PAC_critical_errors_manager::AC_NO_CONNECTION,
                    PAC_critical_errors_manager::AS_IO_COUPLER, wn->number);
                wn->is_set_err = false;
                }
            if ( 100 == val ) //����� ������.
                {
                PAC_critical_errors_manager::get_instance()->reset_global_error(
                    PAC_critical_errors_manager::AC_SERVICE,
                    PAC_critical_errors_manager::AS_IO_COUPLER, wn->number );
                PAC_critical_errors_manager::get_instance()->reset_global_error(
                    PAC_critical_errors_manager::AC_NO_CONNECTION,
                    PAC_critical_errors_manager::AS_IO_COUPLER, wn->number);
                wn->is_set_err = false;
                }
            }
        }

    return 0;
    }

bool PAC_info::is_emulator()
    {
#ifdef DEBUG_NO_IO_MODULES
    return true;
#else
    return false;
#endif
    }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
PAC_info* G_PAC_INFO()
    {
    return PAC_info::get_instance();
    }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
