/* GDK - The GIMP Drawing Kit
 * Copyright (C) 2000 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#include "config.h"

#include "gdkkeysprivate.h"
#include "gdkdisplay.h"
#include "gdkdisplaymanagerprivate.h"


/**
 * SECTION:keys
 * @Short_description: Functions for manipulating keyboard codes
 * @Title: Key Values
 *
 * Key values are the codes which are sent whenever a key is pressed or released.
 * They appear in the #GdkEventKey.keyval field of the
 * #GdkEventKey structure, which is passed to signal handlers for the
 * #GtkWidget::key-press-event and #GtkWidget::key-release-event signals.
 * The complete list of key values can be found in the
 * <filename>&lt;gdk/gdkkeysyms.h&gt;</filename> header file.
 *
 * Key values are regularly updated from the upstream X.org X11 implementation,
 * so new values are added regularly. They will be prefixed with GDK_KEY_ rather
 * than XF86XK_ or XK_ (for older symbols).
 *
 * Key values can be converted into a string representation using
 * gdk_keyval_name(). The reverse function, converting a string to a key value,
 * is provided by gdk_keyval_from_name().
 *
 * The case of key values can be determined using gdk_keyval_is_upper() and
 * gdk_keyval_is_lower(). Key values can be converted to upper or lower case
 * using gdk_keyval_to_upper() and gdk_keyval_to_lower().
 *
 * When it makes sense, key values can be converted to and from
 * Unicode characters with gdk_keyval_to_unicode() and gdk_unicode_to_keyval().
 *
 * <para id="key-group-explanation">
 * One #GdkKeymap object exists for each user display. gdk_keymap_get_default()
 * returns the #GdkKeymap for the default display; to obtain keymaps for other
 * displays, use gdk_keymap_get_for_display(). A keymap
 * is a mapping from #GdkKeymapKey to key values. You can think of a #GdkKeymapKey
 * as a representation of a symbol printed on a physical keyboard key. That is, it
 * contains three pieces of information. First, it contains the hardware keycode;
 * this is an identifying number for a physical key. Second, it contains the
 * <firstterm>level</firstterm> of the key. The level indicates which symbol on the
 * key will be used, in a vertical direction. So on a standard US keyboard, the key
 * with the number "1" on it also has the exclamation point ("!") character on
 * it. The level indicates whether to use the "1" or the "!" symbol.  The letter
 * keys are considered to have a lowercase letter at level 0, and an uppercase
 * letter at level 1, though only the uppercase letter is printed.  Third, the
 * #GdkKeymapKey contains a group; groups are not used on standard US keyboards,
 * but are used in many other countries. On a keyboard with groups, there can be 3
 * or 4 symbols printed on a single key. The group indicates movement in a
 * horizontal direction. Usually groups are used for two different languages.  In
 * group 0, a key might have two English characters, and in group 1 it might have
 * two Hebrew characters. The Hebrew characters will be printed on the key next to
 * the English characters.
 * </para>
 *
 * In order to use a keymap to interpret a key event, it's necessary to first
 * convert the keyboard state into an effective group and level. This is done via a
 * set of rules that varies widely according to type of keyboard and user
 * configuration. The function gdk_keymap_translate_keyboard_state() accepts a
 * keyboard state -- consisting of hardware keycode pressed, active modifiers, and
 * active group -- applies the appropriate rules, and returns the group/level to be
 * used to index the keymap, along with the modifiers which did not affect the
 * group and level. i.e. it returns "unconsumed modifiers." The keyboard group may
 * differ from the effective group used for keymap lookups because some keys don't
 * have multiple groups - e.g. the Enter key is always in group 0 regardless of
 * keyboard state.
 *
 * Note that gdk_keymap_translate_keyboard_state() also returns the keyval, i.e. it
 * goes ahead and performs the keymap lookup in addition to telling you which
 * effective group/level values were used for the lookup. #GdkEventKey already
 * contains this keyval, however, so you don't normally need to call
 * gdk_keymap_translate_keyboard_state() just to get the keyval.
 */


enum {
  DIRECTION_CHANGED,
  KEYS_CHANGED,
  STATE_CHANGED,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GdkKeymap, gdk_keymap, G_TYPE_OBJECT)

static void
gdk_keymap_class_init (GdkKeymapClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  /**
   * GdkKeymap::direction-changed:
   * @keymap: the object on which the signal is emitted
   * 
   * The ::direction-changed signal gets emitted when the direction of
   * the keymap changes. 
   *
   * Since: 2.0
   */
  signals[DIRECTION_CHANGED] =
    g_signal_new ("direction-changed",
		  G_OBJECT_CLASS_TYPE (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GdkKeymapClass, direction_changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE,
		  0);
  /**
   * GdkKeymap::keys-changed:
   * @keymap: the object on which the signal is emitted
   *
   * The ::keys-changed signal is emitted when the mapping represented by
   * @keymap changes.
   *
   * Since: 2.2
   */
  signals[KEYS_CHANGED] =
    g_signal_new ("keys-changed",
		  G_OBJECT_CLASS_TYPE (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GdkKeymapClass, keys_changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE,
		  0);

  /**
   * GdkKeymap::state-changed:
   * @keymap: the object on which the signal is emitted
   *
   * The ::state-changed signal is emitted when the state of the
   * keyboard changes, e.g when Caps Lock is turned on or off.
   * See gdk_keymap_get_caps_lock_state().
   *
   * Since: 2.16
   */
  signals[STATE_CHANGED] =
    g_signal_new ("state_changed",
                  G_OBJECT_CLASS_TYPE (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GdkKeymapClass, state_changed),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 
                  0);
}

static void
gdk_keymap_init (GdkKeymap *keymap)
{
}

/* Other key-handling stuff
 */

/**
 * gdk_keyval_convert_case:
 * @symbol: a keyval
 * @lower: (out): return location for lowercase version of @symbol
 * @upper: (out): return location for uppercase version of @symbol
 *
 * Obtains the upper- and lower-case versions of the keyval @symbol.
 * Examples of keyvals are #GDK_KEY_a, #GDK_KEY_Enter, #GDK_KEY_F1, etc.
 */
void
gdk_keyval_convert_case (guint symbol,
                         guint *lower,
                         guint *upper)
{
  GdkDisplayManager *manager = gdk_display_manager_get ();

  GDK_DISPLAY_MANAGER_GET_CLASS (manager)->keyval_convert_case (manager, symbol, lower, upper);
}

/**
 * gdk_keyval_to_upper:
 * @keyval: a key value.
 *
 * Converts a key value to upper case, if applicable.
 *
 * Returns: the upper case form of @keyval, or @keyval itself if it is already
 *   in upper case or it is not subject to case conversion.
 */
guint
gdk_keyval_to_upper (guint keyval)
{
  guint result;
  
  gdk_keyval_convert_case (keyval, NULL, &result);

  return result;
}

/**
 * gdk_keyval_to_lower:
 * @keyval: a key value.
 *
 * Converts a key value to lower case, if applicable.
 *
 * Returns: the lower case form of @keyval, or @keyval itself if it is already
 *  in lower case or it is not subject to case conversion.
 */
guint
gdk_keyval_to_lower (guint keyval)
{
  guint result;
  
  gdk_keyval_convert_case (keyval, &result, NULL);

  return result;
}

/**
 * gdk_keyval_is_upper:
 * @keyval: a key value.
 *
 * Returns %TRUE if the given key value is in upper case.
 *
 * Returns: %TRUE if @keyval is in upper case, or if @keyval is not subject to
 *  case conversion.
 */
gboolean
gdk_keyval_is_upper (guint keyval)
{
  if (keyval)
    {
      guint upper_val = 0;
      
      gdk_keyval_convert_case (keyval, NULL, &upper_val);
      return upper_val == keyval;
    }
  return FALSE;
}

/**
 * gdk_keyval_is_lower:
 * @keyval: a key value.
 *
 * Returns %TRUE if the given key value is in lower case.
 *
 * Returns: %TRUE if @keyval is in lower case, or if @keyval is not
 *   subject to case conversion.
 */
gboolean
gdk_keyval_is_lower (guint keyval)
{
  if (keyval)
    {
      guint lower_val = 0;
      
      gdk_keyval_convert_case (keyval, &lower_val, NULL);
      return lower_val == keyval;
    }
  return FALSE;
}

/**
 * gdk_keymap_get_default:
 *
 * Returns the #GdkKeymap attached to the default display.
 *
 * Returns: (transfer none): the #GdkKeymap attached to the default display.
 */
GdkKeymap*
gdk_keymap_get_default (void)
{
  return gdk_keymap_get_for_display (gdk_display_get_default ());
}

/**
 * gdk_keymap_get_direction:
 * @keymap: a #GdkKeymap
 *
 * Returns the direction of effective layout of the keymap.
 *
 * Returns: %PANGO_DIRECTION_LTR or %PANGO_DIRECTION_RTL
 *   if it can determine the direction. %PANGO_DIRECTION_NEUTRAL
 *   otherwise.
 **/
PangoDirection
gdk_keymap_get_direction (GdkKeymap *keymap)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->get_direction (keymap);
}

/**
 * gdk_keymap_have_bidi_layouts:
 * @keymap: a #GdkKeymap
 *
 * Determines if keyboard layouts for both right-to-left and left-to-right
 * languages are in use.
 *
 * Returns: %TRUE if there are layouts in both directions, %FALSE otherwise
 *
 * Since: 2.12
 **/
gboolean
gdk_keymap_have_bidi_layouts (GdkKeymap *keymap)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->have_bidi_layouts (keymap);
}

/**
 * gdk_keymap_get_caps_lock_state:
 * @keymap: a #GdkKeymap
 *
 * Returns whether the Caps Lock modifer is locked.
 *
 * Returns: %TRUE if Caps Lock is on
 *
 * Since: 2.16
 */
gboolean
gdk_keymap_get_caps_lock_state (GdkKeymap *keymap)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->get_caps_lock_state (keymap);
}

/**
 * gdk_keymap_get_num_lock_state:
 * @keymap: a #GdkKeymap
 *
 * Returns whether the Num Lock modifer is locked.
 *
 * Returns: %TRUE if Num Lock is on
 *
 * Since: 3.0
 */
gboolean
gdk_keymap_get_num_lock_state (GdkKeymap *keymap)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->get_num_lock_state (keymap);
}

/**
 * gdk_keymap_get_entries_for_keyval:
 * @keymap: a #GdkKeymap
 * @keyval: a keyval, such as %GDK_a, %GDK_Up, %GDK_Return, etc.
 * @keys: (out) (array length=n_keys) (transfer full): return location
 *     for an array of #GdkKeymapKey
 * @n_keys: return location for number of elements in returned array
 *
 * Obtains a list of keycode/group/level combinations that will
 * generate @keyval. Groups and levels are two kinds of keyboard mode;
 * in general, the level determines whether the top or bottom symbol
 * on a key is used, and the group determines whether the left or
 * right symbol is used. On US keyboards, the shift key changes the
 * keyboard level, and there are no groups. A group switch key might
 * convert a keyboard between Hebrew to English modes, for example.
 * #GdkEventKey contains a %group field that indicates the active
 * keyboard group. The level is computed from the modifier mask.
 * The returned array should be freed
 * with g_free().
 *
 * Return value: %TRUE if keys were found and returned
 **/
gboolean
gdk_keymap_get_entries_for_keyval (GdkKeymap     *keymap,
                                   guint          keyval,
                                   GdkKeymapKey **keys,
                                   gint          *n_keys)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->get_entries_for_keyval (keymap, keyval, keys, n_keys);
}

/**
 * gdk_keymap_get_entries_for_keycode:
 * @keymap: a #GdkKeymap
 * @hardware_keycode: a keycode
 * @keys: (out) (array length=n_entries) (transfer full): return
 *     location for array of #GdkKeymapKey, or %NULL
 * @keyvals: (out) (array length=n_entries) (transfer full): return
 *     location for array of keyvals, or %NULL
 * @n_entries: length of @keys and @keyvals
 *
 * Returns the keyvals bound to @hardware_keycode.
 * The Nth #GdkKeymapKey in @keys is bound to the Nth
 * keyval in @keyvals. Free the returned arrays with g_free().
 * When a keycode is pressed by the user, the keyval from
 * this list of entries is selected by considering the effective
 * keyboard group and level. See gdk_keymap_translate_keyboard_state().
 *
 * Returns: %TRUE if there were any entries
 **/
gboolean
gdk_keymap_get_entries_for_keycode (GdkKeymap     *keymap,
                                    guint          hardware_keycode,
                                    GdkKeymapKey **keys,
                                    guint        **keyvals,
                                    gint          *n_entries)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->get_entries_for_keycode (keymap, hardware_keycode, keys, keyvals, n_entries);
}

/**
 * gdk_keymap_lookup_key:
 * @keymap: a #GdkKeymap
 * @key: a #GdkKeymapKey with keycode, group, and level initialized
 *
 * Looks up the keyval mapped to a keycode/group/level triplet.
 * If no keyval is bound to @key, returns 0. For normal user input,
 * you want to use gdk_keymap_translate_keyboard_state() instead of
 * this function, since the effective group/level may not be
 * the same as the current keyboard state.
 *
 * Return value: a keyval, or 0 if none was mapped to the given @key
 **/
guint
gdk_keymap_lookup_key (GdkKeymap          *keymap,
                       const GdkKeymapKey *key)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->lookup_key (keymap, key);
}

/**
 * gdk_keymap_translate_keyboard_state:
 * @keymap: a #GdkKeymap
 * @hardware_keycode: a keycode
 * @state: a modifier state
 * @group: active keyboard group
 * @keyval: (out) (allow-none): return location for keyval, or %NULL
 * @effective_group: (out) (allow-none): return location for effective
 *     group, or %NULL
 * @level: (out) (allow-none): return location for level, or %NULL
 * @consumed_modifiers: (out) (allow-none): return location for modifiers
 *     that were used to determine the group or level, or %NULL
 *
 * Translates the contents of a #GdkEventKey into a keyval, effective
 * group, and level. Modifiers that affected the translation and
 * are thus unavailable for application use are returned in
 * @consumed_modifiers.
 * See <xref linkend="key-group-explanation"/> for an explanation of
 * groups and levels. The @effective_group is the group that was
 * actually used for the translation; some keys such as Enter are not
 * affected by the active keyboard group. The @level is derived from
 * @state. For convenience, #GdkEventKey already contains the translated
 * keyval, so this function isn't as useful as you might think.
 *
 * <note><para>
 * @consumed_modifiers gives modifiers that should be masked out
 * from @state when comparing this key press to a hot key. For
 * instance, on a US keyboard, the <literal>plus</literal>
 * symbol is shifted, so when comparing a key press to a
 * <literal>&lt;Control&gt;plus</literal> accelerator &lt;Shift&gt; should
 * be masked out.
 * </para>
 * <informalexample><programlisting>
 * &sol;* We want to ignore irrelevant modifiers like ScrollLock *&sol;
 * &num;define ALL_ACCELS_MASK (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK)
 * gdk_keymap_translate_keyboard_state (keymap, event->hardware_keycode,
 *                                      event->state, event->group,
 *                                      &amp;keyval, NULL, NULL, &amp;consumed);
 * if (keyval == GDK_PLUS &&
 *     (event->state &amp; ~consumed &amp; ALL_ACCELS_MASK) == GDK_CONTROL_MASK)
 *   &sol;* Control was pressed *&sol;
 * </programlisting></informalexample>
 * <para>
 * An older interpretation @consumed_modifiers was that it contained
 * all modifiers that might affect the translation of the key;
 * this allowed accelerators to be stored with irrelevant consumed
 * modifiers, by doing:</para>
 * <informalexample><programlisting>
 * &sol;* XXX Don't do this XXX *&sol;
 * if (keyval == accel_keyval &&
 *     (event->state &amp; ~consumed &amp; ALL_ACCELS_MASK) == (accel_mods &amp; ~consumed))
 *   &sol;* Accelerator was pressed *&sol;
 * </programlisting></informalexample>
 * <para>
 * However, this did not work if multi-modifier combinations were
 * used in the keymap, since, for instance, <literal>&lt;Control&gt;</literal>
 * would be masked out even if only <literal>&lt;Control&gt;&lt;Alt&gt;</literal>
 * was used in the keymap. To support this usage as well as well as
 * possible, all <emphasis>single modifier</emphasis> combinations
 * that could affect the key for any combination of modifiers will
 * be returned in @consumed_modifiers; multi-modifier combinations
 * are returned only when actually found in @state. When you store
 * accelerators, you should always store them with consumed modifiers
 * removed. Store <literal>&lt;Control&gt;plus</literal>,
 * not <literal>&lt;Control&gt;&lt;Shift&gt;plus</literal>,
 * </para></note>
 *
 * Return value: %TRUE if there was a keyval bound to the keycode/state/group
 **/
gboolean
gdk_keymap_translate_keyboard_state (GdkKeymap       *keymap,
                                     guint            hardware_keycode,
                                     GdkModifierType  state,
                                     gint             group,
                                     guint           *keyval,
                                     gint            *effective_group,
                                     gint            *level,
                                     GdkModifierType *consumed_modifiers)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->translate_keyboard_state (keymap,
								  hardware_keycode,
								  state,
								  group,
								  keyval,
								  effective_group,
								  level,
								  consumed_modifiers);
}

/**
 * gdk_keymap_add_virtual_modifiers:
 * @keymap: a #GdkKeymap
 * @state: (out): pointer to the modifier mask to change
 *
 * Adds virtual modifiers (i.e. Super, Hyper and Meta) which correspond
 * to the real modifiers (i.e Mod2, Mod3, ...) in @modifiers.
 * are set in @state to their non-virtual counterparts (i.e. Mod2,
 * Mod3,...) and set the corresponding bits in @state.
 *
 * GDK already does this before delivering key events, but for
 * compatibility reasons, it only sets the first virtual modifier
 * it finds, whereas this function sets all matching virtual modifiers.
 *
 * This function is useful when matching key events against
 * accelerators.
 *
 * Since: 2.20
 */
void
gdk_keymap_add_virtual_modifiers (GdkKeymap       *keymap,
			          GdkModifierType *state)
{
  GDK_KEYMAP_GET_CLASS(keymap)->add_virtual_modifiers (keymap, state);
}

/**
 * gdk_keymap_map_virtual_modifiers:
 * @keymap: a #GdkKeymap
 * @state: (out): pointer to the modifier state to map
 *
 * Maps the virtual modifiers (i.e. Super, Hyper and Meta) which
 * are set in @state to their non-virtual counterparts (i.e. Mod2,
 * Mod3,...) and set the corresponding bits in @state.
 *
 * This function is useful when matching key events against
 * accelerators.
 *
 * Returns: %TRUE if no virtual modifiers were mapped to the
 *     same non-virtual modifier. Note that %FALSE is also returned
 *     if a virtual modifier is mapped to a non-virtual modifier that
 *     was already set in @state.
 *
 * Since: 2.20
 */
gboolean
gdk_keymap_map_virtual_modifiers (GdkKeymap       *keymap,
                                  GdkModifierType *state)
{
  return GDK_KEYMAP_GET_CLASS(keymap)->map_virtual_modifiers (keymap, state);
}

/**
 * gdk_keyval_name:
 * @keyval: a key value
 *
 * Converts a key value into a symbolic name.
 *
 * The names are the same as those in the
 * <filename>&lt;gdk/gdkkeysyms.h&gt;</filename> header file
 * but without the leading "GDK_KEY_".
 *
 * Return value: (transfer none): a string containing the name of the key,
 *     or %NULL if @keyval is not a valid key. The string should not be
 *     modified.
 */
gchar*
gdk_keyval_name (guint keyval)
{
  GdkDisplayManager *manager = gdk_display_manager_get ();

  return GDK_DISPLAY_MANAGER_GET_CLASS (manager)->get_keyval_name (manager, keyval);
}

/**
 * gdk_keyval_from_name:
 * @keyval_name: a key name
 *
 * Converts a key name to a key value.
 *
 * The names are the same as those in the
 * <filename>&lt;gdk/gdkkeysyms.h&gt;</filename> header file
 * but without the leading "GDK_KEY_".
 *
 * Returns: the corresponding key value, or %GDK_KEY_VoidSymbol
 *     if the key name is not a valid key
 */
guint
gdk_keyval_from_name (const gchar *keyval_name)
{
  GdkDisplayManager *manager = gdk_display_manager_get ();

  return GDK_DISPLAY_MANAGER_GET_CLASS (manager)->lookup_keyval (manager, keyval_name);
}
