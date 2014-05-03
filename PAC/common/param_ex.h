/// @file param_ex.h
/// @brief �������� �������� �������, ������� ��������� ������ � �����������.
///
/// @author  ������ ������� ���������.
///
/// @par �������� �������� �������������:
/// @c DEBUG - ���������� � ������� ���������� ���������� � �������.@n
/// @c KEY_CONFIRM - ������� � ���������� ����������� ��������� �� �������
/// �������.@n@n
/// @c USE_SIMPLE_DEV_ERRORS   - ���������� � ������� ������ ���������.@n
/// @c USE_NO_TANK_COMB_DEVICE - ���������� ��� �������� ���� � ��������. ���
/// ���������� ������� ��������� ��� ���������� ������� ���������������
/// ��������.
///
/// @par ������� ������:
/// @$Rev$.\n
/// @$Author$.\n
/// @$Date::                     $.

#ifndef PARAMS_EX_H
#define PARAMS_EX_H

#ifdef WIN_OS
#include <locale.h>
#endif //WIN_OS

#include <math.h>
#include <string.h>

#include "base_mem.h"
#include "g_device.h"

#ifdef __BORLANDC__
#pragma option -w-inl
#pragma option -w-ccc
#pragma option -w-rch
#endif // __BORLANDC__

class saved_params_u_int_4;
//-----------------------------------------------------------------------------
/// @brief ������ � �����������.
///
/// ������ ��� �������� �������� ��� ������ � �����������. �����������
/// ������������ ������ ����� �������� ����������� �����.
class params_manager
    {
    public:
        enum CONSTANTS
            {
            C_TOTAL_PARAMS_SIZE = 1024 * 30, ///< ����� ������ ������ ����������.

            C_SYS_MEM_SIZE    = 10,          ///< ������ ��� �������� CRC � �.�.

            C_CRC_OFFSET      = 0,
            C_LAST_IDX_OFFSET = 4
            };

        /// @brief ���������� ������������ ��������� ������ ��� ������ �
        /// �����������.
        ///
        /// @return - ��������� �� ������������ ������ ������ @ref
        /// params_manager.
        static params_manager* get_instance();

        /// @brief ������������� �������� ����������.
        ///
        /// ���������� ���������� �� EEPROM � ������ ����������, ��������
        /// ����������� �����. ����������� ����� ������� ��
        /// ��������� @ref C_TOTAL_PARAMS_SIZE.
        ///
        /// @param project_id - ����� �������. ��� �������� ���������� �
        /// ���������� ����������� ���������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������ ����������� �����.
        int init( unsigned int project_id );

        /// @brief ������������� ������������� �������� ����������.
        ///
        /// ���� ��� ���������� ���������� �� EEPROM ��������� ������ (�����
        /// @ref init), ��������� �������� ���������� �� ���������
        /// ����� ������ �������� �������.
        ///
        /// @param auto_init_params - �������� �� ������� init_params �������
        /// tank � comb.
        /// @param auto_init_work_params - �������� �� ������� init_work_params
        /// ������� tank � comb.
        /// @param custom_init_params_function - ���������������� �������
        /// ������������� ����������.
        void final_init( int auto_init_params = 1,
            int auto_init_work_params = 1,
            void ( *custom_init_params_function )() = 0 );

        /// @brief ������ ���������� � EEPROM.
        ///
        /// ������ ���������� �� ������� ���������� � EEPROM.
        ///
        /// @param start_pos - ����� �������, � �������� ������ ������
        /// ���������� (��� ������ ������ ������ ���������).
        /// @param count - ���������� ������������ ����.
        void save( int start_pos = 0, int count = 0 );

        /// @brief ����� ����������� ����� (��� ������������� ���������� ��
        /// ���������).
        void reset_CRC();

        /// @brief ��������� ��������� �� ���� ������ ����������.
        ///
        /// @param size      - ������ ����� ������ � ������.
        /// @param [out] start_pos - ��������� ������ � ���������� �������
        /// ����������. ��������� ��� ���������� ������ ���������� � ������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������ ����������� �����.
        char* get_params_data( int size, int &start_pos );

        ~params_manager();

        enum PARAMS
            {
            P_COUNT = 1,

            P_IS_RESET_PARAMS = 1,
            };

        saved_params_u_int_4 *par;

        int save_params_as_Lua_str( char* str );

        int restore_params_from_server_backup( char *backup_str );

        // ������������ ����������� �����.
        u_int_2 solve_CRC();

    private:
        /// @brief �������� �����������.
        ///
        /// ��� ������ ������� ������������ ����������� ����� @ref get_instance.
        params_manager();

        /// ����������� ��������� ������ ��� ������ �������.
        static auto_smart_ptr< params_manager > instance;

        /// ������� ������ ����������.
        char params[ C_TOTAL_PARAMS_SIZE ];

        /// ����� ���������� ����������� ���������. ������������ ��� ��������
        /// ���������� ������ @ref parameters.
        u_int last_idx;

        /// ������� ���������� �������� ���������� (������������� �����������
        /// �����).
        int loaded;

        u_int project_id;   ///< ����� ������� (��� ������������ ����������).

        memory_range *params_mem; ///< ������ ����������.
        memory_range *CRC_mem;    ///< ������ ����������� �����.

        /// @brief �������� ����������� �����.
        ///
        /// ������� ����������� ����� � ��������� �� �� ���������,
        /// ���������� � NVRAM (2 ������ ����� NVRAM).
        ///
        /// @return 0 - ��.
        /// @return 1 - ������ ����������� �����.
        int check_CRC();

        /// @brief ������� ����������� ����� � ������ �� � NVRAM.
        void make_CRC();
    };
//-----------------------------------------------------------------------------
/// @brief ������ � �������� ����������.
///
/// ������ ��� �������� ���������� ����� ����������. ����������� ��������
/// ������� ����� ������ ( [] ).

template < class type, bool is_float > class parameters
    {
    private:
        char name[ 20 ];

    public:
        /// @brief ��������� �������� ����� �������� ��������������.
        ///
        /// @param index - ������ ��������.
        ///
        /// @return - �������� �������� � �������� ��������. ���� ������
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// stub ( �������� 0 ).
        type& operator[] ( unsigned int index )
            {
            if ( index <= count && index > 0 )
                {
                return values[ index - 1 ];
                }
#ifdef DEBUG
            else
                {
                if ( 0 == index )
                    {
                    Print( "\"%s\" parameters[] - error: index = %u\n",
                        name, index );
                    }
                else
                    {
                    Print( "\"%s\" parameters[] - error: index[ %u ] > count [ %u ]\n",
                        name, index, count );
                    }
                }
#endif // DEBUG

            return stub;
            }

        /// @brief ��������� �������� ����� �������� ��������������.
        ///
        /// @param index - ������ ��������.
        ///
        /// @return - �������� �������� � �������� ��������. ���� ������
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// stub ( �������� 0 ).
        const type& operator[] ( unsigned int index ) const
            {
            if ( index <= count && index > 0 )
                {
                return values[ index - 1 ];
                }
#ifdef DEBUG
            else
                {
                Print( "parameters[] - error: index[ %u ] > count [ %u ]\n",
                    index, count );
                }
#endif // DEBUG

            return stub;
            }
        /// @brief ��������� �������� ����� ������.
        ///
        /// @param idx - ������ ��������.
        ///
        /// @return - �������� �������� � �������� ��������. ���� ������
        /// ������� �� ��������, ������������ �������� �������� - ���� @ref
        /// stub ( �������� 0 ).
        type get_val( int idx )
            {
            return this->operator []( idx );
            }

        /// @brief ��������� ���� ���������� � ������� ��������.
        virtual void reset_to_0()
            {
            for ( u_int i = 1; i <= count; i++ )
                {
                values[ i - 1 ] = 0;
                }
            }

        /// @brief ��������� ���������� ����������.
        ///
        /// @return - ���������� ����������.
        u_int get_count() const
            {
            return count;
            }

        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        /// @param name  - ��� �������.
        /// @param value - ��������� �� ����� ��� �������� �������� ����������.
        parameters( int count, const char *name, type *value = 0 ): count( count ),
            values( value )
            {
            stub = 0;
            this->name[ 0 ] = 0;
            strcpy( this->name, name );

#ifdef DEBUG_IDE
            if ( 0 == count )
                {
                Print( "parameters(...) - error: count = 0!\n" );
                }
            //debug_break;
#endif // DEBUG_IDE
            is_delete = 0;
            if ( count > 0 && 0 == values )
                {
                is_delete = 1;
                values = new type[ count ];
                memset( values, 0, count * sizeof( type ) );
                }
            }

        virtual ~parameters()
            {
            if ( 1 == is_delete && count > 0 && values > 0 )
                {
                delete [] values;
                values = 0;
                count = 0;
                }
            }

        virtual void print() const
            {
            Print( "\"%s\"\t - ", name );
            for ( u_int i = 1; i <= count; i++ )
                {
                Print( "[%d]=", i );
                if ( is_float )
                	{
                    Print( "%.2f,", values[ i - 1 ] );
                	}
                else
                    {
                    Print( "%d,", values[ i - 1 ] );
                    }
                }
            Print( "\n" );
            }

        int save_device_ex( char *buff, const char *prefix, const char *new_name )
            {
            int res = sprintf( buff, "%s%s = \n", prefix, new_name );
            res += save_dev( buff + res, prefix );

            return res;
            }

        int save_device( char *buff, const char *prefix )
            {
            int res = sprintf( buff, "%s%s = \n", prefix, name );
            res += save_dev( buff + res, prefix );

            return res;
            }

    protected:
        char is_delete; ///< ������� �������� ������ ��� �������� �������.

        /// @brief ��������� ��������� �� ����� ��� �������� ��������
        /// ����������.
        ///
        /// @return - ��������� �� ����� ��� �������� �������� ����������.
        type* get_values()
            {
            return values;
            }

        /// �������� ��� ��������� ����� ������ � ������� �� ��������.
        type         stub;

        unsigned int count;     ///< ���������� ���������.
        type         *values;   ///< ��������� �� ������ �������� ���������.

    private:
        int save_dev( char *buff, const char *prefix )
            {
            int answer_size = sprintf( buff, "%s\t{\n%s\t",
                prefix, prefix );

#ifdef WIN_OS
			setlocale(LC_NUMERIC, "C");
#endif

            for ( u_int i = 1; i <= count; i++ )
                {
                if ( is_float )
                    {
                    float val  = ( float ) get_val( i );
                    if ( 0. == val )
                        {
                        answer_size += sprintf( buff + answer_size, "0, " );
                        }
                    else
                        {
                        double tmp;
                        if ( modf( val, &tmp ) == 0 )
                            {
                            answer_size += sprintf( buff + answer_size, "%d, ",
                                ( int ) val );
                            }
                        else
                            {
                            answer_size += sprintf( buff + answer_size, "%.2f, ",
                                val );
                            }
                        }
                    }
                else
                    {
                    answer_size += sprintf( buff + answer_size, "%u, ",
                        ( u_int ) get_val( i ) );
                    }
                }

            answer_size += sprintf( buff + answer_size, "\n%s\t},\n", prefix );
            return answer_size;
            }

    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������� ������� ���������� ���� float.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����.
class run_time_params_float: public parameters < float, true >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        run_time_params_float( int count ):parameters < float, true >( count, "RT_PAR_F" )
            {
            }

    protected:

        float get_val( int idx )
            {
            return parameters< float, true >::get_val( idx );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ����������� ������� ���������� ���� @ref u_int_4.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����.
class run_time_params_u_int_4: public parameters < u_int_4, false >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        /// @param name  - ��� ����������.
        run_time_params_u_int_4( int count,
            const char *name = "RT_PAR_UI" ) : parameters < u_int_4, false >( count, name )
            {
            }

        virtual ~run_time_params_u_int_4()
            {
            }

    protected:
        u_int_4 get_val( int idx )
            {
            return parameters< u_int_4, false >::get_val( idx );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ �����������.
///
/// ������ ��������� ���������� �� ������ ����� ��������������� ����. ���
/// ������������ PAC �� �������� �����������.
template < class type, bool is_float > class saved_params:
public parameters < type, is_float >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        /// @param name  - ��� �������.
        saved_params( int count, const char *name ) : parameters < type, is_float >(
            count, name,
            ( type* ) params_manager::get_instance()->get_params_data(
            count * sizeof( type ), start_pos ) )
            {
            }

        virtual ~saved_params()
            {
            }

        /// @brief ���������� �������� ��������� � ����������������� ������.
        ///
        /// �������� ������� ����� ������ ��������� �������� ��������� ������
        /// � ������, ��� ��� ��� ���������� ��� � ����������������� ������ ����
        /// ������������ ������ �����.
        type save( u_int idx, type value )
            {
            if ( idx > 0 && idx <= parameters< type, is_float >::get_count() )
                {
                idx--;
                parameters< type, is_float >::get_values()[ idx ] = value;

                params_manager::get_instance()->save(
                    start_pos + idx * sizeof( type ), sizeof( type ) );
                }
#ifdef DEBUG
            else
                {
                if ( 0 == idx )
                    {
                    Print( "parameters:save - index = %u\n",
                        idx );
                    }
                else
                    {
                    Print( "parameters:save - index[ %u ] > count [ %u ]\n",
                        idx, parameters< type, is_float >::get_count() );
                    }
                }
#endif // DEBUG
            return value;
            }

        /// @brief ���������� �������� ���� ���������� � ����������������� ������.
        ///
        /// �������� ������� ����� ������ ��������� �������� ��������� ������
        /// � ������, ��� ��� ��� ���������� ��� � ����������������� ������ ����
        /// ������������ ������ �����.
        int save_all()
            {
            params_manager::get_instance()->save(
                start_pos,  parameters< type, is_float >::get_count() * sizeof( type ) );

            return 0;
            }

        /// @brief ��������� ���� ���������� � ������� �������� � ����������
        /// � ����������������� ������.
        void reset_to_0()
            {
            for ( u_int i = 0; i <  parameters< type, is_float >::get_count(); i++ )
                {
                parameters< type, is_float >::get_values()[ i ] = 0;
                }

            params_manager::get_instance()->save(
                start_pos, sizeof( type ) * parameters< type, is_float >::get_count() );
            }

    private:
        /// ������ ������ �������� � ����� �������, ��� ���������� ��������
        /// ��������� � ����������������� ������ (@ref save).
        int start_pos;
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ ����������� ���� @ref u_int_4.
class saved_params_u_int_4: public saved_params < u_int_4, false >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params_u_int_4( int count ) :
          saved_params < u_int_4, false >( count, "S_PAR_UI" )
              {
              }

          virtual ~saved_params_u_int_4()
              {
              }

    protected:
        u_int_4 get_val( int idx )
            {
            return saved_params< u_int_4, false >::get_val( idx );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ������ � ������������ ����������� ���� float.
class saved_params_float: public saved_params < float, true >
    {
    public:
        /// @brief �����������.
        ///
        /// @param count - ���������� ����������.
        saved_params_float( int count ):
          saved_params < float, true >( count, "S_PAR_F" )
              {
              }

          virtual ~saved_params_float()
              {
              }

    protected:
        float get_val( int idx )
            {
            return saved_params< float, true >::get_val( idx );
            }
    };
//-----------------------------------------------------------------------------
/// @brief ����� ��� ������������ ������� ������ � �����������.
class params_test
    {
    public:
        /// @brief ������������ ������� ������ � �����������.
        ///
        /// @return 0 - ��.
        /// @return 1 - ������.
        static int make_test();
    };
//-----------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma option -w.inl
#pragma option -w.ccc
#pragma option -w.rch
#endif // __BORLANDC__

#endif // PARAMS_EX_H
