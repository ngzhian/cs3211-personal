package hw1;

public class Q2 implements Runnable {
  private static final int AMOUNT = 500;
  private static int a = 1000;
  private static int b = 1000;
  private boolean AtoB;

  public Q2(boolean AtoB) {
    this.AtoB = AtoB;
  }

  public static void main(String args[]) {
    Thread transferA = new Thread(new Q2(true));
    Thread transferB = new Thread(new Q2(false));
    transferA.start();
    transferB.start();
    try {
      transferA.join();
      transferB.join();
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
    System.out.println("from: " + a + " to: " + b);
  }

  /*
   * BROKEN PROGRAM
   * 
   * @Override public void run() { if (AtoB) { if (a > AMOUNT) { int newval = a
   * - AMOUNT; b = b + AMOUNT; a = newval; } else { Thread.yield(); } } else {
   * if (b > AMOUNT) { int newval = b - AMOUNT; a = a + AMOUNT; b = newval; }
   * else { Thread.yield(); } } }
   */

  /* Good program */
  @Override
  public void run() {
    if (AtoB) {
      if (a > AMOUNT) {
        b = b + AMOUNT;
        a = a - AMOUNT;
      } else {
        Thread.yield();
      }
    } else {
      if (b > AMOUNT) {
        a = a + AMOUNT;
        b = b - AMOUNT;
      } else {
        Thread.yield();
      }
    }
  }
}