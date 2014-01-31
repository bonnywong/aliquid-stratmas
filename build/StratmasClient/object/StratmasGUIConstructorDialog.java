// 	$Id: StratmasGUIConstructorDialog.java,v 1.1 2006/03/22 14:30:51 dah Exp $
/*
 * @(#)StratmasGUIConstructor.java
 */

package StratmasClient.object;

import StratmasClient.object.type.Declaration;
import StratmasClient.object.type.Type;
import StratmasClient.TypeSelector;
import javax.swing.JPanel;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.JButton;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.AbstractAction;
import javax.swing.JComboBox;
import java.util.Vector;
import java.util.Enumeration;
import java.awt.event.ActionEvent;
import javax.swing.BoxLayout;

/**
 * A JDialog used to create a StratmasObject.
 *
 * @version 1, $Date: 2006/03/22 14:30:51 $
 * @author  Daniel Ahlin
*/
public class StratmasGUIConstructorDialog extends JDialog
{
    /**
     * The constructor this dialog wraps.
     */
    StratmasGUIConstructor constructor;

    /**
     * Indicates that this dialog was cancelled.
     */
    boolean cancelled = false;

    /**
     * Creates a new Dialog for the specified constructor
     *
     * @param constructor constructor to wrap.
     */
    StratmasGUIConstructorDialog(StratmasGUIConstructor constructor)
    {
	super((java.awt.Frame) null, "Create " + 
	      constructor.getDeclaration().getType().getName(), true);
	this.constructor = constructor;	
    } 
    
    /**
     * Returns the StratmasObject this dialog is expected to create,
     * or null if none has been created.
     */
    public StratmasObject getStratmasObject()
    {
	if (!cancelled) {
	    return constructor.getStratmasObject();
	} else {
	    return null;
	}
    }

    /**
     * Call to cancel dialog.
     */
    public void cancel() 
    {
	cancelled = true;
	dispose();
    }
}
