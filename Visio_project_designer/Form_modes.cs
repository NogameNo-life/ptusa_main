﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using wago;
using tech_device;
using visio_prj_designer;

namespace Visio_project_designer
    {
    public partial class Form_modes : Form
        {
        public visio_addin addin = Globals.visio_addin;
        public ListBox lbox;                //  Удалить
        public TreeNode[] Prop_struct;      //  Стандартная структура характеристик режима
        public TreeNode cur_node;           //  Текущий узел дерева режимов
        
        public const int first_lavel = 2;   // Первый уровень хар-к для которого можно добавлять устр.

        public Form_modes()
            {
            InitializeComponent();

            addin.is_selecting_dev = true;

            //   Заполнение структуры характеристик на основе визуального компонента
            Prop_struct = new TreeNode[ treeView_prop.Nodes.Count ];

            for ( int i = 0; i < treeView_prop.Nodes.Count; i++ )
                {
                Prop_struct[ i ] = treeView_prop.Nodes[ i ];
                }
            //-----------------------------------------------------------------------

            //   Заполнение дерева объектов и их режимов (для Ограничений)
            treeView2.Nodes.Clear();

            for ( int i = 0; i < addin.g_objects.Count; i++ )
                {
                treeView2.Nodes.Add( addin.g_objects[ i ].get_name() );

                for ( int j = 0; j < addin.g_objects[ i ].mode_mas.Count; j++ )
                    {
                    treeView2.Nodes[ i ].Nodes.Add( addin.g_objects[ i ].mode_mas[ j ].name );
                    }
                }
            //-----------------------------------------------------------------------
            }

        private void Form_modes_Shown( object sender, EventArgs e )
            {
            this.Text = "Данные объекта " + addin.cur_sel_obj.get_name();

            //  Заполнение списка устройств
            dev_list.Items.Clear();
            foreach	( device dev in Globals.visio_addin.g_devices )
			    {
                dev_list.Items.Add( dev.get_name() + ": " + dev.description );
                }
            //-----------------------------------------------------------------------

            //  Заполнение списка режимов и шагов для выбранного объекта
            treeView_modes.Nodes.Clear();

            for ( int i = 0; i < addin.cur_sel_obj.mode_mas.Count; i++ )
                {
                TreeNode temp_node = treeView_modes.Nodes.Add(
                    Convert.ToString( addin.cur_sel_obj.mode_mas[ i ].no ) +
                    ". " + addin.cur_sel_obj.mode_mas[ i ].name );

                for ( int j = 0; j < addin.cur_sel_obj.mode_mas[ i ].step.Count; j++ )
                    {
                    temp_node.Nodes.Add(
                        Convert.ToString( addin.cur_sel_obj.mode_mas[ i ].step[ j ].no ) +
                        ". " + addin.cur_sel_obj.mode_mas[ i ].step[ j ].name );
                    }
                }

            //  Заполнение таблицы с параметрами
            numericUpDown1.Value = addin.cur_sel_obj.param_list.Count;
            //dataGridView1.RowCount = addin.cur_sel_obj.param_list.Count;

            for ( int i = 0; i < addin.cur_sel_obj.param_list.Count; i++ )
                {
                dataGridView1.Rows[ i ].Cells[ 0 ].Value = Convert.ToString( i );
                dataGridView1.Rows[ i ].Cells[ 1 ].Value = addin.cur_sel_obj.param_list[ i ][ 0 ];
                dataGridView1.Rows[ i ].Cells[ 2 ].Value = addin.cur_sel_obj.param_list[ i ][ 1 ];
                }
            //-----------------------------------------------------------------------

            }
        //---------------------------------------------------------------------

        private void Form_modes_FormClosing( object sender, FormClosingEventArgs e )
            {
            //  Обработка закрытия формы (Сохранять изменения или нет)

            switch ( MessageBox.Show( "Сохранить изменения?", "", MessageBoxButtons.YesNoCancel ) )
                {
                case DialogResult.Yes:
/*
                //  Сохранение списка режимов и их характеристик
                    for ( int i = 0; i < treeView_modes.Nodes.Count; i++ )
                        {
                        Save_ModeData( addin.cur_sel_obj.mode_mas[ i ] );

                             
                        //******************
                        //public T_Object.mode Save_ModeData( T_Object.mode cur_mode )

                        cur_mode.no = treeView_modes.Nodes[ i ].Index;
                        cur_mode.name = treeView_modes.Nodes[ i ].Text;

                        for ( int k = 0; k < cur_mode.Nodes.Count; k++ )
                            {                        

                            }
                        
                        for ( int j = 0; j < treeView_modes.Nodes[ i ].Nodes.Count; j++ )
                            {

                            }
                        //******************


                        }
*/
                    //  Сохранение параметров
                    addin.cur_sel_obj.param_list.Clear();

                    for ( int i = 0; i < numericUpDown1.Value; i++ )
                        {
                        addin.cur_sel_obj.param_list.Add( new string[ 2 ] );

                        addin.cur_sel_obj.param_list[ i ][ 0 ] =
                            dataGridView1.Rows[ i ].Cells[ 1 ].Value.ToString();
                        addin.cur_sel_obj.param_list[ i ][ 1 ] =
                            dataGridView1.Rows[ i ].Cells[ 2 ].Value.ToString();
                        }

                    addin.is_selecting_dev = false;
                    break;

                case DialogResult.No:
                    addin.is_selecting_dev = false;
                    break;

                case DialogResult.Cancel:
                    e.Cancel = true;
                    break;

                }
            }

        //---------------------------------------------------------------------

        private void treeView_modes_MouseClick( object sender, MouseEventArgs e )
            {
            MouseButtons key = e.Button;
            switch ( key )
                {
                case MouseButtons.Left:
                    break;

                case MouseButtons.Right:
                    //contextMenuStrip1.Show( MousePosition.X, MousePosition.Y );

                    break;
                }
            }

        private void treeView_modes_BeforeSelect( object sender, TreeViewCancelEventArgs e )
            {
            //  Работа с предыдущим режимом - Сохранение даннах дерева
            if ( cur_node != null )
                {
                if ( cur_node.Level == 0 )
                    {
                    Save_PropTree(
                        addin.cur_sel_obj.mode_mas[ addin.cur_mode ] );
                    }
                else
                    {
                    Save_PropTree(
                        addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ] );
                    }
                }    
            //=======================================================================
            }
        //---------------------------------------------------------------------

        private void treeView_modes_AfterSelect( object sender, TreeViewEventArgs e )
            {
            //  Работа с выбранным режимом
            cur_node = treeView_modes.SelectedNode;

            addin.cur_mode = -1;    //0
            addin.cur_step = -1;    //0

            //  Определяем режим и шаг
            if ( cur_node.Level == 0 )
                {
                addin.cur_mode = cur_node.Index;
                addin.cur_step = -1;
                //cur_oper = addin.cur_sel_obj.mode_mas[ addin.cur_mode ];

                //  Заполнение списков с устройствами
                if ( addin.cur_mode < addin.cur_sel_obj.mode_mas.Count )
                    {
                    //filling_dev_list( addin.cur_sel_obj.mode_mas[ addin.cur_mode ] );
                    addin.cur_sel_obj.mode_mas[ addin.cur_mode ] =
                    Refresh_prop_tree( addin.cur_sel_obj.mode_mas[ addin.cur_mode ] );
                    }
                }
            else
                {
                addin.cur_mode = cur_node.Parent.Index;
                addin.cur_step = cur_node.Index;
                //cur_oper = addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ];

                //  Заполнение списков с устройствами
                if ( ( addin.cur_mode < addin.cur_sel_obj.mode_mas.Count )
                    && ( addin.cur_step < addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step.Count )
                   )
                    {
                    //filling_dev_list( addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ] );
                    addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ] =
                    Refresh_prop_tree( addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ] );
                    }
                }


            //  Выполнить проверку необходимости показывать все виды характеристик
            //      если ни одна хар-ка в группе не задана - снять галочку (в соотв. меню)
            //  *****
/*            
            bool flag_no_prop;      // флаг необходимости отображения данного типа хар-к 
            //  Проходим по типам характеристик
            for ( int i = 0; i < treeView_prop.Nodes.Count; i++ )
                {
                //  Выделяем текущий узел
                treeView_prop.SelectedNode = treeView_prop.Nodes[ i ];
                
                flag_no_prop = false;
                
                //  Проходим по характеристикам
                for ( int j = 0; j < treeView_prop.Nodes[ i ].Nodes.Count; j++ )
                    {
                    //  Если в характеристике задан хотябы один элемент
                    if ( treeView_prop.Nodes[ i ].Nodes[ j ].Nodes.Count > 0 )
                        {
                        flag_no_prop = true;    //  То устанавливаем флаг
                        break;
                        }
                    }

                //  Находим соответствующий узлу дерева пункт меню  
                ToolStripItem qwer = contextMenuStrip2.Items.Find( 
                                                treeView_prop.Nodes[ i ].Name, true )[ 0 ];
                
                if ( flag_no_prop == true )
                    {
                    ((ToolStripMenuItem ) qwer).Checked = true;
                    }
                else
                    {
                    ((ToolStripMenuItem ) qwer).Checked = false;
                    }

                }
*/

            }

        private void treeView_prop_MouseClick( object sender, MouseEventArgs e )
            {
            MouseButtons key = e.Button;
            switch ( key )
                {
                case MouseButtons.Left:
                    break;

                case MouseButtons.Right:
                    if ( ( ( TreeView ) sender ).SelectedNode.Level < first_lavel )
                        {
                        contextMenuStrip2.Show( MousePosition.X, MousePosition.Y );
                        }
                    else
                        {
                        contextMenuStrip4.Show( MousePosition.X, MousePosition.Y );
                        }
                    break;
                }
            }

        private void treeView_prop_AfterSelect( object sender, TreeViewEventArgs e )
            {
            if ( ( ( ( TreeView ) sender ).SelectedNode.Level == first_lavel )
              && ( ( ( TreeView ) sender ).SelectedNode.Parent.Text == "Ограничения" )
               )
                {
                treeView2.Visible = true;
                }
            else
                {
                treeView2.Visible = false;
                }
            }

        private void treeView_prop_KeyDown( object sender, KeyEventArgs e )
            {
            switch ( e.KeyCode )
                {
                case Keys.Delete:
                    if ( ( ( ( TreeView ) sender ).SelectedNode != null )
                        && ( ( ( TreeView ) sender ).SelectedNode.Level > first_lavel )
                        )
                        {
                        ( ( TreeView ) sender ).SelectedNode.Remove();
                        }
                    break;

                case Keys.Insert:
                    break;

                case Keys.Enter:
                    break;
                }
            }

        private void ContextMenuStrip2_set_def_prop_Click( object sender, EventArgs e )
            {
            tech_device.T_Object.mode cur_mode;

            if ( addin.cur_step >= 0 )
                {
                cur_mode = addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ];
                }
            else
                {
                cur_mode = addin.cur_sel_obj.mode_mas[ addin.cur_mode ];
                }

            //  Если нет, то задаем стандартную структуру
            cur_mode.TreeView_params.Nodes.Clear();

            //  Если структура пустая, то забиваем ее стандартными пунктами
            for ( int i = 0; i < Prop_struct.Length; i++ )
                {
                cur_mode.TreeView_params.Nodes.Add( Prop_struct[ i ] );
                }

            cur_mode = Refresh_prop_tree( cur_mode );
            }

        //---------------------------------------------------------------------

        private void ContextMenuStrip3_insert_mode_Click( object sender, EventArgs e )
            {
            //  Создаем режим
            T_Object.mode new_mode = new T_Object.mode();

            //  НЕ создаем структуру характеристик режима, она будет создана
            //  автоматически при обновлении дерева характеристик если она == NULL
            //  new_mode.TreeView_params = new TreeView();

            //  Добавляем режим в структуру
            if ( treeView_modes.SelectedNode.Level == 0 )
                {
                new_mode.set_attribute( treeView_modes.SelectedNode.Index + 1, "Новый режим" );
                //  Добавляем в структуру
                addin.cur_sel_obj.mode_mas.Insert( treeView_modes.SelectedNode.Index + 1, new_mode );
                }
            else
                {
                new_mode.set_attribute( treeView_modes.SelectedNode.Index + 1, "Новый шаг" );
                //  Добавляем в структуру
                addin.cur_sel_obj.mode_mas[ treeView_modes.SelectedNode.Parent.Index ].step.Insert(
                                            treeView_modes.SelectedNode.Index + 1, new_mode );
                }

            //  Обновляем дерево
            Refresh_mode_tree();
            }
        //---------------------------------------------------------------------

        private void ContextMenuStrip3_insert_step_Click( object sender, EventArgs e )
            {
            //  Создаем режим
            T_Object.mode new_mode = new T_Object.mode();

            //  Добавляем режим в структуру
            if ( treeView_modes.SelectedNode.Level == 0 )
                {
                //  0-ой шаг, т.к. при обновлении будет присвоен нужный номер
                new_mode.set_attribute( 0, "Новый шаг" );

                //  Добавляем в структуру
                addin.cur_sel_obj.mode_mas[ treeView_modes.SelectedNode.Index ].step.Add( new_mode );
                }
            else
                {
                //  0-ой шаг, т.к. при обновлении будет присвоен нужный номер
                new_mode.set_attribute( 0, "Новый шаг" );

                //  Добавляем в структуру
                addin.cur_sel_obj.mode_mas[ treeView_modes.SelectedNode.Parent.Index ].step.Insert(
                                            treeView_modes.SelectedNode.Index + 1, new_mode );
                }

            //  Обновляем дерево
            Refresh_mode_tree();
            }
        //---------------------------------------------------------------------

        private void ContextMenuStrip3_delete_mode_Click( object sender, EventArgs e )
            {
            if ( treeView_modes.SelectedNode.Level == 0 )
                {
                addin.cur_sel_obj.mode_mas.Remove( addin.cur_sel_obj.mode_mas[ addin.cur_mode ] );
                }
            else
                {
                addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step.Remove(
                                addin.cur_sel_obj.mode_mas[ addin.cur_mode ].step[ addin.cur_step ] );
                }

            //  Обновляем дерево
            //treeView_modes.SelectedNode.Remove();
            Refresh_mode_tree();
            }

        private void ContextMenuStrip4_select_on_map_Click( object sender, EventArgs e )
            {
            //  Переход на схему и последующий выбор
            Globals.visio_addin.vis_main_ribbon.insert_dev_list();
            }

        //---------------------------------------------------------------------

        //---------------------------------------------------------------------

        public void add_dev_in_tree( device dev )
            {
            //  В текущую выделенную характеристику добавляем выбранное устройство
            if ( ( treeView_prop.SelectedNode != null )
              && ( treeView_prop.SelectedNode.Level >= first_lavel ) )
                {
                treeView_prop.SelectedNode.Nodes.Add( new TreeNode( dev.get_name() ) );
                treeView_prop.SelectedNode.Expand();
                }
            else
                {
                MessageBox.Show( "Не выбран список для заполнения устройствами!" );
                }
            }
        //---------------------------------------------------------------------

        private void dataGridView1_CellEnter( object sender, DataGridViewCellEventArgs e )
            {
            dataGridView1.Rows[ e.RowIndex ].Cells[ 0 ].Value = Convert.ToString( e.RowIndex );
            }
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------

        private void dev_list_DoubleClick( object sender, EventArgs e )
            {
            device cur_dev = addin.g_devices.Find( delegate( device dev )
                {
                    return ( ( dev.get_name() + ": " + dev.description ) ==
                             ( dev_list.SelectedItem.ToString() ) );
                }
            );

            if ( cur_dev != null )
                add_dev_in_tree( cur_dev );
            }
        //  Заполнение списков с устройствами
        private void filling_dev_list( T_Object.mode cur_mode )
            {
            //  Для работы с деревьями

/*          //  Для работы со списками
            listBox_open.Items.Clear();

            if ( cur_mode.on_device != null )
                {
                foreach ( tech_device.device dev in cur_mode.on_device )
                    {
                    if ( addin.g_devices.Contains( dev ) ) 
                        {
                        listBox_open.Items.Add( //"V" + Convert.ToString( dev.get_n() ) + 
                                            dev.get_name() + ": " +
                                            dev.description );
                        }
                    }
                }
            
            listBox_close.Items.Clear();

            if ( cur_mode.off_device != null )
                {
                foreach ( tech_device.device dev in cur_mode.off_device )
                    {
                    if ( addin.g_devices.Contains( dev ) ) 
                        {
                        listBox_close.Items.Add( //"V" + Convert.ToString( dev.get_n() ) + 
                                             dev.get_name() + ": " +
                                             dev.description );
                        }
                    }
                }

*/
            }
        //---------------------------------------------------------------------

        private void numericUpDown1_ValueChanged( object sender, EventArgs e )
            {
            dataGridView1.RowCount = Convert.ToInt32( numericUpDown1.Value ) + 1;

            for ( int i = 0; i < dataGridView1.RowCount; i++ )
                {
                dataGridView1.Rows[ i ].Cells[ 0 ].Value = Convert.ToString( i );
                }
            }
        //---------------------------------------------------------------------

        private void Refresh_mode_tree()
            {
            TreeNode new_node;

            treeView_modes.Nodes.Clear();

            //  Добавляем режимы
            for ( int i = 0; i < addin.cur_sel_obj.mode_mas.Count; i++ )
                {
                addin.cur_sel_obj.mode_mas[ i ].set_attribute( i, null );

                new_node = treeView_modes.Nodes.Add(
                    addin.cur_sel_obj.mode_mas[ i ].no + ". " +
                    addin.cur_sel_obj.mode_mas[ i ].name );

                //  Добавляем шаги
                for ( int j = 0; j < addin.cur_sel_obj.mode_mas[ i ].step.Count; j++ )
                    {
                    addin.cur_sel_obj.mode_mas[ i ].step[ j ].set_attribute( i, null );

                    new_node.Nodes.Add(
                        addin.cur_sel_obj.mode_mas[ i ].step[ j ].no + ". " +
                        addin.cur_sel_obj.mode_mas[ i ].step[ j ].name );
                    }
                }

            }
        //---------------------------------------------------------------------

        private T_Object.mode Refresh_prop_tree( T_Object.mode cur_mode )
            {
            treeView_prop.BeginUpdate();
            treeView_prop.Nodes.Clear();

            //  Перед заполнением дерева проверяем есть ли структура дерева в режиме
            //  Если нет, то задаем стандартную структуру
            if ( cur_mode.TreeView_params == null )
                {
                cur_mode.TreeView_params = new TreeView();
                }

            //  Если структура пустая, то забиваем ее стандартными пунктами
            if ( cur_mode.TreeView_params.Nodes.Count == 0 )
                {
                for ( int i = 0; i < Prop_struct.Length; i++ )
                    {
                    cur_mode.TreeView_params.Nodes.Add( Prop_struct[ i ] );
                    }
                }

            //  Заполнение дерева характеристик
            for ( int i = 0; i < cur_mode.TreeView_params.Nodes.Count; i++ )
                {
                treeView_prop.Nodes.Add(
                    ( ( TreeNode ) cur_mode.TreeView_params.Nodes[ i ].Clone() ) );
                treeView_prop.Nodes[ i ].Expand();
                }

            treeView_prop.EndUpdate();

            return cur_mode;
            }
        //---------------------------------------------------------------------

        private void Save_PropTree( T_Object.mode cur_mode )
            {
            if ( cur_mode.TreeView_params != null )
                {
                cur_mode.TreeView_params.Nodes.Clear();

                for ( int i = 0; i < treeView_prop.Nodes.Count; i++ )
                    {
                    cur_mode.TreeView_params.Nodes.Add(
                        ( ( TreeNode ) treeView_prop.Nodes[ i ].Clone() ) );
                    }
                }
            }

        //---------------------------------------------------------------------

        private void ToolStripMenuItem_CheckStateChanged( object sender, EventArgs e )
            {
/*
            int i;

            //  Определяем какой узел выбран
            if ( treeView_prop.SelectedNode != null )
                {
                i = treeView_prop.SelectedNode.Index;

                //  В зависимости от выбора добавляем или удаляем характеристики в дерево
                if ( ( ( ToolStripMenuItem ) sender ).Checked == true )
                    {
                    treeView_prop.Nodes.Insert( i, Prop_struct[ i ] );
                    }
                else
                    {
                    treeView_prop.Nodes.Remove( Prop_struct[ i ] );
                    }
                }
*/
            }
        //---------------------------------------------------------------------

        private void listBox_open_DoubleClick( object sender, EventArgs e )
            {
            /*            
                        if ( listBox_open.SelectedIndex < 0 )
                            {
                            //  Запоминаем какой список выделен в данный момент
                            if ( sender is ListBox )
                                {
                                lbox = sender as ListBox;
                                }
                            else
                                {
                                lbox = null;
                                }

                            //  Показываем контекстное меню (указываем координаты)
                            contextMenuStrip1.Show( MousePosition.X, MousePosition.Y );
                            }
                        else
                            {
                            //  Определяем что за устройство выбрано (если выбрано)
             // *****
                            if ( addin.cur_step >= 0 )
                                {
                                //cur_oper 
                                }
                            //     listBox_open.SelectedIndex;


                            //  Переходим на схему
                            //  *****
 
                            //  Выделяем устройство на схеме
                            //  *****

                            }
             */
            }
        //---------------------------------------------------------------------

        private void listView_modes_SelectedIndexChanged( object sender, EventArgs e )
            {
            //  Заполнение списков устройств

            }
        //---------------------------------------------------------------------

        }
    }
